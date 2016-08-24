// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <CmdMessenger.h>
#include <SoftwareSerial.h> // required for secondary Serial port to the light controller

// Required by MIRF
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

/**
   Project Files
*/
#include "build.h"  // Project settings such as pins or build flags
#include "Accelerometer.h"
#include "GameModes.h"
#include "Relay.h"
#include "fire-patterns.h"
#include "Settings.h"


// MUST BE BELOW build.h
#ifdef MAD_MAIN_LOGGING
#define LOGN(x)  Serial.println (x)
#define LOG(x)  Serial.print (x)
#else
#define LOGN(x)
#define LOG(x)
#endif


/**
   Static Members
*/
static Relay * r0 = 0;
static Relay * r1 = 0;

static Accelerometer * accelerometer = 0;
static unsigned char fire_pattern = 0;		// id of the current pattern

static bool serial_input_enabled = true;	// QA before setting to false
static bool wireless_input_enabled = true;	// QA before setting to false

// Serial port connected to the Lighting controller
SoftwareSerial SlaveSerial(0, 1); // RX, TX

// Payload size used in nRF24L01 communication
#define PAYLOAD_SIZE 4

/**
   Function Declerations
*/
void relay_setup();
void wireless_setup();
const unsigned char * current_fire_pattern();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
   Main Arduino Functions
*/
void setup() {

  relay_setup();          // call before delay because relays are active low so we want to set to be HIGH as soon as possible
  Serial.begin(9600);     // Begin serial
  LOGN(F("It's a MAD World!"));

  // configure the SlaveControllerSerial
  if ( serial_input_enabled ) {
    SlaveSerial.begin(4800);
  }
  

  blinkLEDOnBootup();     // Blink the LED to give some time to reset Teensy if needed

  loadConfig();           // load important values about angles and other app settings that need to persist

  LOGN(F("Begin Accelerometer Setup"));
  delay(100);

  // Initalize the Accelerometer module
  accelerometer = new Accelerometer();
  accelerometer->setup();

  // Wireless configure
  LOGN(F("Begin Wireless Setup"));
  wireless_setup();

  // Unused LEDs setup as output LEDs
#ifdef MAD_USE_UNUSED_LEDS_FOR_STATUS
  pinMode(UNUSED_RELAY_PIN_0, OUTPUT);
  pinMode(UNUSED_RELAY_PIN_1, OUTPUT);
  digitalWrite(UNUSED_RELAY_PIN_0, HIGH); // active low
  digitalWrite(UNUSED_RELAY_PIN_1, HIGH); // active low
#endif

  if ( accelerometer->working ) {
    LOGN(F("Accelerometer setup complete"));
  } else {
    LOGN(F("Accelerometer setup failed."));
  }

  // On boot up, send the mode
  send_current_mode();
  delay(100);

  LOGN(F("Setup complete"));
}



void loop() {
  
  // Must call loop() prior to using accelerometer to ensure it has updated itself internally
  accelerometer->loop();

  // look for transitions from NONE to SIDE0TOP or SIDE1TOP
  if ( accelerometer->position_changed && accelerometer->last_position == AccelerometerPositionNone) {

    if ( accelerometer->position == AccelerometerPositionSide0Top ) {   // RED
      poof_triggered_by_angle(0x01);

    } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {  // BLUE
      poof_triggered_by_angle(0x02);
    }
  }


  if ( serial_input_enabled ) {             // Check for Serial input commands

    byte incomingByte;
    bool hadData = false;
    while (SlaveSerial.available() > 0) {        // read the entire serial, and only process the last command
      incomingByte = SlaveSerial.read();
#if MAD_MAIN_LOGGING
      Serial.print("Read a byte: "); Serial.println(incomingByte, HEX);
#endif
      hadData = true;
    }
    if ( hadData ) {
      cmd_handle_from_serial(incomingByte);
    }

  }
  
  if ( wireless_input_enabled ) {    // Check for wireless commands.
    // Note: Interrupt pin is not connected on the SeeSaw Rev1.0 board.

    // Finish sending first
    bool wasSending = false;
    while (Mirf.isSending()) {  // TODO: add timeout?
      wasSending = true;
    }
    if ( wasSending ) {
      // copied from library example. Adds a delay after sending 
      // for some reason, perhaps to let the data get pushed out fully
      delay(10);  
    }

    // Check for new data
    byte payload[PAYLOAD_SIZE];
    bool hadData = false;

    // only process the last payload received in case there is more than 1 for some reason
    // TODO: Assumption of only receiving 1 payload at a time could be tested.
    while ( Mirf.dataReady()) {

      LOGN("Wireless data received...");
      Mirf.getData(payload);
#if MAD_MAIN_LOGGING
      Serial.print("Wireless: Reading a payload: ");
      for ( byte i = 0; i < PAYLOAD_SIZE; i++) {
        Serial.print(payload[i], HEX);
      }
      Serial.println("");
#endif
      hadData = true;
    }

    if ( hadData ) {
      cmd_handle_from_wireless(payload);
    }
  }

  // Relays call loop() on each main loop so they can update their internal state.
  // Do this after modifying the relay (eg. by calling setOnWithPattern())
  r0->loop();
  r1->loop();

  // prevent us from spending all the time inside loop() so interrupt based things can happen
  delay(10);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
   Command handlers for commands  that can be sent to this board via Serial, RF, or some other comm channel
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
   Passes a command from Serial "API" into the command handlers
*/
void cmd_handle_from_serial(byte commandByte ) {
  switch (commandByte) {
    case SERIAL_API_COMMAND_TRIGGER_POOF_LEFT: 
      cmd_poof(0x01); 
      break;
    case SERIAL_API_COMMAND_TRIGGER_POOF_RIGHT: 
      cmd_poof(0x02); 
      break;
    case SERIAL_API_COMMAND_TRIGGER_POOF_BOTH:
      cmd_poof(0x03); 
      break;
    default:
      LOGN("Unsupported command from serial.");
      break;
  }

}

/*
   Passes a command from Wireless "API" into the command handlers
    Byte 0: Header (0x55)
    Byte 1: Header 2 (0xAA) or Remote ID
    Byte 2: Address/Command byte
    Byte 3: Tail (0xFF)
*/
void cmd_handle_from_wireless(byte * commandRcvd ) {

  // we respect the mode here if in a mode where the Lighting contorller sends poofs only. 
  // This could be changed. Remote beats all ;)

  if ( commandRcvd[0] != 0x55 ) {
    LOGN("Wireless error: Header is not 0x55");
    return;
  } else if ( commandRcvd[3] != 0xFF ) {
    LOGN("Wireless error: Tail is not 0xFF");
    return;
  }
#if MAD_MAIN_LOGGING
  Serial.print("Wireless received 0x"); Serial.print(commandRcvd[2], HEX);
  Serial.print(" from remote ID# 0x"); Serial.println(commandRcvd[1], HEX);
#endif

  switch (commandRcvd[2]) {
    case WIRELESS_API_COMMAND_TRIGGER_POOF_LEFT: 
      cmd_poof(0x01); 
      break;
    case WIRELESS_API_COMMAND_TRIGGER_POOF_RIGHT: 
      cmd_poof(0x02); 
      break;
    case WIRELESS_API_COMMAND_TRIGGER_POOF_BOTH: 
      cmd_poof(0x03); 
      break;
    case WIRELESS_API_COMMAND_SAVE_ANGLE: 
      cmd_save_current_seesaw_angle(); 
      break;
    case WIRELESS_API_COMMAND_MODE_CHANGE_NORMAL:
      cmd_change_mode(GameModeNone);
      break;
    case WIRELESS_API_COMMAND_MODE_CHANGE_LIGHTING:
      cmd_change_mode(GameModeLighting);
      break;
    case WIRELESS_API_COMMAND_MODE_BRIGHTNESS_UP:
      cmd_send_brightness_change(true);
      break;
    case WIRELESS_API_COMMAND_MODE_BRIGHTNESS_DOWN:
      cmd_send_brightness_change(false);
      break;
    default:
      LOGN("Unsupported command from wireless.");
      break;
  }
}

/**
 * Inform the lighting controller of a brightness change
 */
void cmd_send_brightness_change(bool isUp) {
  byte cmd = isUp ? SERIAL_API_COMMAND_BRIGHT : SERIAL_API_COMMAND_DIM;
  if ( serial_input_enabled ) {
      #if MAD_MAIN_LOGGING
      Serial.print("Reporting brightness change with command 0x"); Serial.println(cmd, HEX);
      #endif
      SlaveSerial.write(cmd);
  }  
}

/** 
 * Changes mode and reports the change via the Serial out API.
 */
void cmd_change_mode(GameMode newMode) {

  if ( newMode == currentGameMode ) {
    LOGN(F("Already in mode. not changing."));
    send_current_mode();  // send the mode anyway.
    return;    
  }

  switch (newMode) {

    case GameModeNone:
      currentGameMode = GameModeNone;
      break;

    case GameModeLighting:
      currentGameMode = GameModeLighting;
      break;

    default:
      break;
  }
  send_current_mode();
}

/*
 * Calibrates the trigger angles of the seesaw.
*/

void cmd_save_current_seesaw_angle() {
  // NOTE: could call loop() here to update angle, but assuming its been updated recently as it is always called inside loop()
  LOGN(F("Handling Command: Save SeeSaw Angle"));
  accelerometer->storeCurrentAngleForSide();
  flashUnusedLEDsBlocking();
}

/**
 * Changes the default fire pattern used when the seesaw is trigger to fire (e.g. by angle trigger)
 */
void cmd_change_fire_pattern() {
  fire_pattern = (fire_pattern + 1)%COUNT_TRIGGER_FIRE_PATTERNS ;
}

/*
   Does a default poof pattern on 1 or both of the poofers. Uses a bitmask to specify which ones to poof.
   Values for `which`:
   0x01 : Relay 0
   0x02 : Relay 1
   0x03 : Both Relays
*/
void cmd_poof(byte which) {

  // Just logging
  LOG(F("Handling Command: Poof "));
  if (( which & 0x01) && (which & 0x02)) {
    LOGN(F("Both 0x03"));
  } else if ( which & 0x01 ) {
    LOGN(F("LEFT 0x01"));
  } else if ( which & 0x02 ) {
    LOGN(F("RIGHT 0x02"));
  }

  // Handle the command
  if ( which & 0x01 ) {
    r0->setOnWithPattern(current_fire_pattern());
  }
  if ( which & 0x02 ) {
    r1->setOnWithPattern(current_fire_pattern());
  }

  // Ok to skip since this function is called from loop().
  // --- Call loop so the relays update internally
  //  r0->loop();
  //  r1->loop();
}


#define SpecialPoofShaveShortCut  0x00
#define SpecialPoofJaws           0x01
#define SpecialBeethoven          0x02
#define SpecialRapidFireLeft      0x03
#define SpecialRapidFireRight     0x04
#define SpecialRapidFireBothQuick 0x05

#define SpecialPoofCount          6

/**
 * Called each time special poof command is seen by wireless. 
 * Responsible for cycling through the various patterns we have.
 */
void cmd_special_poof() {
  static byte poofId = 0;
  special_poof_with_type(poofId % SpecialPoofCount);
  poofId++;
}

/**
 * Expose interfaces to call some of the more complicated fire patterns.
 */
void special_poof_with_type(byte poof) {
  
  switch (poof) {
    
    case SpecialPoofShaveShortCut:
      r0->setOnWithPattern(SHAVE_LEFT);
      r1->setOnWithPattern(SHAVE_RIGHT);
      break;
    case SpecialPoofJaws:
      r0->setOnWithPattern(JAWS_LEFT);
      r1->setOnWithPattern(JAWS_RIGHT);
      break;
    case SpecialBeethoven:
      r0->setOnWithPattern(BEETHOVEN_LEFT);
      r1->setOnWithPattern(BEETHOVEN_RIGHT);
      break;
    case SpecialRapidFireLeft:
      r0->setOnWithPattern(RAPID_BURST_100MS);
      break;
    case SpecialRapidFireRight:
      r1->setOnWithPattern(RAPID_BURST_100MS);
      break;
    case SpecialRapidFireBothQuick:
      r0->setOnWithPattern(RAPID_BURST_SUPERFAST);
      r1->setOnWithPattern(RAPID_BURST_SUPERFAST);
      break;
    default:
      Serial.println("Unrecognized special poof value");
      break;    
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Sends the current mode using the Serial API
 */
void send_current_mode() {
	byte cmd = serialCommandToReportModeChange(currentGameMode);
	if ( cmd ) {
	    if ( serial_input_enabled ) {
	      #if MAD_MAIN_LOGGING
	  		Serial.print("Reporting mode with command 0x"); Serial.println(cmd, HEX);
	      #endif
	  		SlaveSerial.write(cmd);
	    }
	}  
}

/*
 * Depending on the mode, will either trigger a poof or tell the Lighting controller that it
 * would like to poof (and requires the lighting controller to actually tell it whether to do so)
 *
 * Values for `which`:
 * 		0x01 : Relay 0
 * 		0x02 : Relay 1
 * 		0x03 : Both Relays
 */
void poof_triggered_by_angle(byte which) {

  if ( currentGameModeAllowsImmediatePoof() ) {

    cmd_poof(which);

  } else if ( currentGameModeRequiresSerialAcknowledgementForPoof() ) {

    // send the command to the Serial controller indicating that we would like to poof
    byte serialCommand = serialCommandToRequestPoofAcknowledgement(which);
    
    if ( serialCommand ) {

      if ( serial_input_enabled ) {
        #if MAD_MAIN_LOGGING
        Serial.print("Asking light controller to poof with command: 0x");
        Serial.println(serialCommand, HEX);
        #endif
        SlaveSerial.write(serialCommand);
      }

    } else {

#if MAD_MAIN_LOGGING
      Serial.print("Bad command sent to poof_triggered_by_angle for bitmask: 0x");
      Serial.println(which, HEX);
#endif

    }

  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
   Setup the Relay objects
*/
void relay_setup() {
  // DONT CALL Serial.print() from here, as this is expected to be called prior to Serial being configured.
  r0 = new Relay(RELAY_0_PIN);
  r1 = new Relay(RELAY_1_PIN);
  r0->clearPattern();
  r1->clearPattern();
}

/**
   Configure Wireless
*/
void wireless_setup() {

  // Set pins for the SeeSaw
  Mirf.cePin = 9;
  Mirf.csnPin = 10;

  LOGN(F("Configuring Wirless..."));
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // Configure reciving address.
  Mirf.setRADDR((byte *)"Unit1");

  /*
     Set the payload length to sizeof(unsigned long) the
     return type of millis().

     NB: payload on client and server must be the same.
  */
  Mirf.payload = PAYLOAD_SIZE;

  /*
     Write channel and payload config then power up reciver.
     To change channel:
     Mirf.channel = 10;
     NB: Make sure channel is legal in your area.
  */
  Mirf.config();
  LOGN(F("Wireless config finished."));
}

/*
   Returns the current pattern being used by default for poofs.
*/
#define COUNT_TRIGGER_FIRE_PATTERNS 4;
const unsigned char * current_fire_pattern() {

  switch (fire_pattern) {
    case 0:
      return SINGLE_BURST;
      break;
    case 1:
      return RAPID_BURST_40MS;
      break;
    case 2:
      return RAPID_BURST_100MS;
      break;
    case 3:
      return RAPID_BURST_SUPERFAST;
      break;
  }
  return SINGLE_BURST;  // a good default
}

/**
   Blinks the on-board LED when setup() is called. Give some time to reset Teensy if needed.
   Dont use LED after setup() as pin 13 is also used for the nRF24L01 chip wiring.
*/
void blinkLEDOnBootup() {
  pinMode(LED, OUTPUT);
  for ( int i = 0; i < 4; i++ ) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}

/**
   Used for debugging, we have two PCB LEDs that are not being used.
*/
void flashUnusedLEDsBlocking() {
#ifdef MAD_USE_UNUSED_LEDS_FOR_STATUS
  // Pins are active low
  digitalWrite(UNUSED_RELAY_PIN_0, LOW);
  digitalWrite(UNUSED_RELAY_PIN_1, LOW);
  delay(2000);
  digitalWrite(UNUSED_RELAY_PIN_0, HIGH);
  digitalWrite(UNUSED_RELAY_PIN_1, HIGH);
#endif
}

