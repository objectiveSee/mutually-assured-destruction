// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

/**
 * Project Files
 */
#include "build.h"  // Project settings such as pins or build flags
#include "Accelerometer.h"
#include "Relay.h"
#include "fire-patterns.h"

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

/**
 * Function Declerations
 */
void relay_setup();
const unsigned char * current_burst_pattern();
void loop_led();
void blink_led();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Main Arduino Functions
 */
void setup() {

	Serial.begin(9600);     // Begin serial
  relay_setup();          // call before delay because relays are active low so we want to set to be HIGH as soon as possible
  pinMode(LED, OUTPUT);   // Initalize pin modes
  blinkLEDOnBootup();     // Blink the LED to give some time to reset Teensy if needed
  
#if MAD_LOGGING
  Serial.println(F("It's a MAD World!"));
  Serial.println(F("Begin accel setup"));
  delay(100);
#endif

  // Initalize the Accelerometer module
  accelerometer = new Accelerometer();
  accelerometer->setup();

#if MAD_LOGGING
  Serial.println(F("Accel setup complete"));
#endif
}

void loop() {
  
  // debugShouldBlink();
  // return;

  // Must call loop() prior to using accelerometer objects to ensure they have updated themselves internally
  //  Serial.println(F("loop"));
  accelerometer->loop();
  // accelerometer->log();
  
  if ( accelerometer->position_changed && accelerometer->last_position == AccelerometerPositionNone) {  // look for transitions from NONE to SIDE0TOP or SIDE1TOP

    if ( accelerometer->position == AccelerometerPositionSide0Top ) {
      Serial.println("Pattern 1 On!");
      r0->setOnWithPattern(SINGLE_BUTTON_PRESS);
      
    } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {
      Serial.println("Pattern 2 On!");
      r1->setOnWithPattern(SINGLE_BUTTON_PRESS);

    }
  }

  // Relays call loop() on each main loop so they can update their internal state.
  // Do this after modifying the relay (eg. by calling setOnWithPattern())
  r0->loop();
  r1->loop();

  // LED blinks are timed with the system clock, so call loop to update it.
  loop_led();

  // prevent us from spending all the time inside loop() so interrupt based things can happen
  delay(10);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void debugShouldBlink() {
  //  unsigned long nowMs = millis();
//  static boolean isBlinked = false;
//  boolean shouldBlink = ((nowMs / 3000) % 2);
//  if( shouldBlink != isBlinked ) {
//
//    if ( shouldBlink ) {
//      r0->setOnWithPattern(SINGLE_BUTTON_PRESS);
//      r1->setOnWithPattern(SINGLE_BUTTON_PRESS);
//      Serial.println("Mode changed. Pattern ON");
//    } else {
//      Serial.println("Mode changed. Doing nothing");
//    }
//    isBlinked = shouldBlink;
//  }
//
//  if ( shouldBlink ) {
//    digitalWrite(LED, HIGH);
//  } else {
//    digitalWrite(LED, LOW);
//  }
//  r0->loop();
//  r1->loop();
//  delay(30);
//  return;
delay(30);
}

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
 * Varilables used to keep track of blinking state of LED
 */
static unsigned long led_blink_end = 0;
#define BLINK_DURATION_MS 500

/**
 * Updates the on-board LED as to whether it should be ON or OFF based on the blinking state
 * TODO: Clean this up. Right now it ignores `led_blink_end` and just blinks every second.
 */
void loop_led() {

  unsigned long nowMs = millis();

  boolean shouldBlink = ((nowMs / 1000) % 2);

  //now < led_blink_end 
  if ( shouldBlink) {
    digitalWrite(LED, HIGH); 
  } else {
    digitalWrite(LED, LOW);
  }

}

/**
 * Unused. See TODO in loop_led().
 */
void blink_led() {
  led_blink_end = millis() + BLINK_DURATION_MS;
}

/**
 * Blinks the on-board LED when setup() is called. Give some time to reset Teensy if needed.
 */
void blinkLEDOnBootup() {
  for ( int i = 0; i < 3; i++ ) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }  
}
