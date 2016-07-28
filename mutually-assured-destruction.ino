// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <CmdMessenger.h>

/**
 * Project Files
 */
#include "build.h"  // Project settings such as pins or build flags
#include "Accelerometer.h"
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
 * Static Members
 */
static Relay * r0 = 0;
static Relay * r1 = 0;
// static Button * button0 = 0;
//static Button * button1 = 0;

static Accelerometer * accelerometer = 0;
//static boolean stopped = 0;
static unsigned char burst_mode = 1;

static bool serial_input_enabled = true;
static bool wireless_input_enabled = true;

/**
 * Function Declerations
 */
void relay_setup();
const unsigned char * current_burst_pattern();
//void loop_led();
//void blink_led();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Main Arduino Functions
 */
void setup() {

	Serial.begin(9600);     // Begin serial
  relay_setup();          // call before delay because relays are active low so we want to set to be HIGH as soon as possible

  LOGN(F("It's a MAD World!"));
  
  blinkLEDOnBootup();     // Blink the LED to give some time to reset Teensy if needed
  
  loadConfig(); // load important values about angles and other app settings that need to persist

  LOGN(F("Begin Accelerometer Setup"));
  delay(100);

  // Initalize the Accelerometer module
  accelerometer = new Accelerometer();
  accelerometer->setup();

  if ( accelerometer->working ) {
    LOGN(F("Accelerometer setup complete"));    
  } else {
    LOGN(F("Accelerometer setup failed."));
  }
}

void loop() {
  
  // Must call loop() prior to using accelerometer objects to ensure they have updated themselves internally
  //  Serial.println(F("loop"));
  accelerometer->loop();
  // accelerometer->log();

  // look for transitions from NONE to SIDE0TOP or SIDE1TOP
  if ( accelerometer->position_changed && 
       accelerometer->last_position == AccelerometerPositionNone) {  

    if ( accelerometer->position == AccelerometerPositionSide0Top ) {   // RED
      cmd_poof(0x01);
      
    } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {  // BLUE
      cmd_poof(0x02);
    }
  }


  // Check for Serial input commands
  // TODO: specify Serial
  if ( serial_input_enabled ) {
    byte incomingByte;
    bool hadData = false;
    while (Serial.available() > 0) {    // read the entire serial, and only process the last command
      incomingByte = Serial.read();
  #if MAD_MAIN_LOGGING
      Serial.print("Read a byte: "); Serial.println(incomingByte, HEX);
  #endif
      hadData = true;
    }
    if ( hadData ) {
      cmd_handle_from_serial(incomingByte);
    }
  }

  if ( wireless_input_enabled ) {
    
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
 * Command handlers for commands  that can be sent to this board via Serial, RF, or some other comm channel
 */


/*
 * Passes a byte from Serial "API" into the command handlers
 */
void cmd_handle_from_serial(byte commandByte ) {
  switch(commandByte) {
    case 'L': cmd_poof(0x01); break;
    case 'R': cmd_poof(0x02); break;
    case 'B': cmd_poof(0x03); break;
    case 'A': cmd_save_current_seesaw_angle(); break;
    default:
      LOGN("Unsupported command from serial.");
      break;
  }
}

/*
 * Calibrates the trigger angles of the seesaw.
 */

void cmd_save_current_seesaw_angle() {
  // NOTE: could call loop() here to update angle, but assuming its been updated recently as it is always called inside loop()
  LOGN(F("Handling Command: Save SeeSaw Angle"));
  accelerometer->storeCurrentAngleForSide();
}

/*
 * Does a default poof pattern on 1 or both of the cannons. Uses a bitmask to specify which ones to poof.
 * Values for `which`:
 * 0x01 : Relay 0
 * 0x02 : Relay 1
 * 0x03 : Both Relays
 */
void cmd_poof(byte which) {

  // Just logging
  LOG(F("Handling Command: Poof "));
  if (( which & 0x01) && (which & 0x02)) {
    LOGN(F("Both"));
  } else if ( which & 0x01 ) {
    LOGN(F("LEFT 0x01"));
  } else if ( which & 0x02 ) {
    LOGN(F("RIGHT 0x02"));
  }

  // Handle the command
  if ( which & 0x01 ) {
    r0->setOnWithPattern(SINGLE_BUTTON_PRESS);    
  }
  if ( which & 0x02 ) {
    r1->setOnWithPattern(SINGLE_BUTTON_PRESS);    
  }

  // Ok to skip if this function is called from loop().
  // --- Call loop so the relays update internally
//  r0->loop();
//  r1->loop();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Setup the Relay objects
 */
void relay_setup() {
	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
  r0->clearPattern();
  r1->clearPattern();
}

/*
 * Returns the current pattern being used by default for poofs.
 * TODO: This isn't being used for allowing someone to change the default burst pattern.
 */
const unsigned char * current_burst_pattern() {

  switch (burst_mode) {
    case 1:
      return SINGLE_BURST;
      break;
    case 2:
      return RAPID_BURST_40MS;
      break;
    case 3:
      return RAPID_BURST_100MS;
      break;
    case 4:
      return RAPID_BURST_SUPERFAST;
      break;
  }
  return SINGLE_BURST;
}

/**
 * Blinks the on-board LED when setup() is called. Give some time to reset Teensy if needed.
 * Dont use LED after setup() as pin 13 is also used for the nRF24L01 chip wiring.
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
