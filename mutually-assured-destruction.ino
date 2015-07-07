// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <IRLib.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>


//////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////

#define LED 13

// Relay & Fire
#define RELAY_0_PIN 10
#define RELAY_1_PIN 9
#define FIRE_BURST_SHORT_DURATION 200

// Buttons
#define BUTTON_0_PIN 6
#define BUTTON_1_PIN 5

// Lights
#define NEOPIXEL_PIN_A 4
#define NEOPIXEL_PIN_B -1
#define NEOPIXEL_COUNT 120

// Logging
#define MAD_LOGGING 0
#define MAD_LIGHTS_ENABLED 0

//////////////////////////////////////////////////
// MAD Libraries
//////////////////////////////////////////////////

#include "Accelerometer.h"
#include "Relay.h"
#include "RemoteControl.h"
//#include "Button.h"
#include "Settings.h"
#include "fire-patterns.h"

#if MAD_LIGHTS_ENABLED
#include "Lights.h"
#endif

//////////////////////////////////////////////////
// Static Memory
//////////////////////////////////////////////////

// Static Members
static Relay * r0 = 0;
static Relay * r1 = 0;
//static Button * button0 = 0;
//static Button * button1 = 0;

#if MAD_LIGHTS_ENABLED
static Lights * lights = 0;
#endif

static RemoteControl * remote = 0;
static Accelerometer * accelerometer = 0;
static boolean stopped = 0;
static boolean accel_enabled = true;

// Function Declerations
void relay_setup();
void toggleLightColors (boolean side);

//////////////////////////////////////////////////
//////////////////////////////////////////////////

#pragma mark - Main

void setup() {
  // put your setup code here, to run once:

	Serial.begin(9600);

  // call before delay because relays are active low so we want to set to be HIGH as soon as possible
	relay_setup();

	digitalWrite(LED, HIGH);
	delay(1000);
	digitalWrite(LED, LOW);
	delay(1000);

  loadConfig(); // read persistent settings from Settings.h

#if MAD_LOGGING
  Serial.println("It's a MAD World!");
  delay(100);
#endif

  accelerometer = new Accelerometer();
  accelerometer->setup();

#if MAD_LIGHTS_ENABLED
  lights = new Lights(NEOPIXEL_PIN_A, NEOPIXEL_PIN_B, NEOPIXEL_COUNT);
  lights->on();
  toggleLightColors(true);
#endif

//  button0 = new Button(BUTTON_0_PIN);
//  button1 = new Button(BUTTON_1_PIN);

  remote = new RemoteControl(-1);  // note pin is specified in class
}

void loop() {

  accelerometer->loop();
  // accelerometer->log();
  RemoteCommand last_command = remote->loop();
//  button0->loop();
//  button1->loop();

  // #ifdef MAD_LOGGING
  // static int t = 0;
  // if ( t == 0 ) {
  //   Serial.println("loop");
  // }
  // t = (t+1)%1000;
  // #endif
  // indicates whether we should clear last command
  // boolean clearLastCommand = true;

  // Process remote command:

  if ( last_command == RemoteCommandStop ) {

    stopped = 1;
    r0->clearPattern();
    r1->clearPattern();
    #if MAD_LIGHTS_ENABLED
    lights->off();
    #endif

  } else if ( last_command == RemoteCommandStart ) {

     stopped = 0;
     #if MAD_LIGHTS_ENABLED
     lights->on();
     #endif

  }

  // can only adjust accelerometer when stopped
  if ( stopped ) {

    static float adjustment_value = 0;
    static AccelerometerPosition adjustingSide = AccelerometerPositionNone;

    if ( last_command == RemoteCommandAdjustAccelerometerLeft ) {
#if MAD_LOGGING
      Serial.println("Left Adjustments");
#endif
      adjustingSide = AccelerometerPositionSide0Top;
    } else if ( last_command == RemoteCommandAdjustAccelerometerLeft ) {
      adjustingSide = AccelerometerPositionSide1Top;
    }
  }



  if (!stopped) {

    if ( last_command == RemoteCommandLeft ) {
#if MAD_LOGGING
      Serial.println("Relay 0 on");
#endif
      r0->setOnWithPattern(SINGLE_BUTTON_PRESS);

    } else if ( last_command == RemoteCommandRight ) {

#if MAD_LOGGING
      Serial.println("Relay 1 on");
#endif
      r1->setOnWithPattern(SINGLE_BUTTON_PRESS);

    } else if ( last_command == RemoteCommandBoth ) {

#if MAD_LOGGING
      Serial.println("Both Relays OnForDuration");
#endif
      r0->setOnWithPattern(SINGLE_BUTTON_PRESS);
      r1->setOnWithPattern(SINGLE_BUTTON_PRESS);

    } else if ( last_command == RemoteCommandDigit9 ) {

      r0->setOnWithPattern(RAPID_BURST_A);
      r1->setOnWithPattern(RAPID_BURST_B);

    } else if ( last_command == RemoteCommandDigit8 ) {

      r0->setOnWithPattern(LRM_A);
      r1->setOnWithPattern(LRM_B);

    } else if ( last_command == RemoteCommandDigit7 ) {

      r0->setOnWithPattern(GOLDBERG_A);
      r1->setOnWithPattern(GOLDBERG_B);

    } else if ( last_command == RemoteCommandDigit5 ) {

      r0->setOnWithPattern(BEETHOVEN_LEFT);
      r1->setOnWithPattern(BEETHOVEN_RIGHT);

    } else if ( last_command == RemoteCommandDigit4 ) {

      r0->setOnWithPattern(RAPID_BURST_100MS);
      r1->setOnWithPattern(RAPID_BURST_100MS);

    } else if ( last_command == RemoteCommandDigit3 ) {

      r0->setOnWithPattern(RAPID_BURST_100MS);

    } else if ( last_command == RemoteCommandDigit2 ) {

      r0->setOnWithPattern(RAPID_BURST_40MS);
      r1->setOnWithPattern(RAPID_BURST_40MS);

    } else if ( last_command == RemoteCommandDigit1 ) {

      r0->setOnWithPattern(RAPID_BURST_40MS);

    } else if ( last_command == RemoteCommandToggleAccelerometer ) {
      accel_enabled = !accel_enabled;

    } else {
      // only do accelerometer based stuff if the remote isn't active (?)

      if ( accelerometer->position_changed) {
        if ( accelerometer->last_position == AccelerometerPositionNone ) {
          if ( accelerometer->position == AccelerometerPositionSide0Top ) {
            if ( accel_enabled ) {
              r0->setOnWithPattern(SINGLE_BUTTON_PRESS);
            }
            toggleLightColors(true);
          } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {
            if ( accel_enabled ) {
              r1->setOnWithPattern(SINGLE_BUTTON_PRESS);
            }
            toggleLightColors(false);
          }
        }
      }
    }

  }   /** end code that only runs if !stopped **/

  r0->loop();
  r1->loop();
#if MAD_LIGHTS_ENABLED
  lights->loop();
#endif

  remote->clearCommand();

  delay(10);  // prevent us from spending all the time inside loop() so interrupt based things can happen
}


void toggleLightColors (boolean side) {
#if MAD_LIGHTS_ENABLED
#if MAD_LOGGING
  Serial.print("Light Change "); Serial.println(side?"A":"B");
#endif

  uint32_t colorRed = lights->Color(10,0,0);
  uint32_t colorBlue = lights->Color(0,0,10);

  if ( side ) {
    lights->setColors(colorRed, colorBlue);
  } else {
    lights->setColors(colorBlue, colorRed);
  }
#endif
}


void relay_setup() {
	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
  r0->off();
  r1->off();
}
