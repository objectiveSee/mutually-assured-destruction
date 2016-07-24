// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//////////////////////////////////////////////////
// Pins
//////////////////////////////////////////////////

#define LED 13

// Relay & Fire
#define RELAY_0_PIN 2         //J4:3
#define RELAY_1_PIN 14        //J4:4

// Buttons
#define BUTTON_0_PIN 3    // Input
#define BUTTON_1_PIN 4    // Input

// Swapping w/ Relays
#define BUTTON_0_LED_PIN 7    //J1:1
#define BUTTON_1_LED_PIN 8   //J4:2

//////////////////////////////////////////////////
// Logging
//////////////////////////////////////////////////

// Logging
#define MAD_LOGGING 1

//////////////////////////////////////////////////
// Project Files
//////////////////////////////////////////////////

#include "Accelerometer.h"
#include "Relay.h"
#include "fire-patterns.h"

//////////////////////////////////////////////////
// Static Memory
//////////////////////////////////////////////////

// Static Members
static Relay * r0 = 0;
static Relay * r1 = 0;
// static Button * button0 = 0;
//static Button * button1 = 0;

//static RemoteControl * remote = 0;
static Accelerometer * accelerometer = 0;
//static boolean stopped = 0;
static unsigned char burst_mode = 1;

// Function Declerations
void relay_setup();
const unsigned char * current_burst_pattern();
void loop_led();
void blink_led();

//////////////////////////////////////////////////
//////////////////////////////////////////////////

// Main

void setup() {

  // Begin serial
	Serial.begin(9600);

  // call before delay because relays are active low so we want to set to be HIGH as soon as possible
	relay_setup();

  // Initalize pin modes
//  pinMode(RELAY_0_PIN, OUTPUT); // not needed
//  pinMode(RELAY_1_PIN, OUTPUT); // not needed
  pinMode(LED, OUTPUT);

//  pinMode(BUTTON_0_LED_PIN, OUTPUT);
//  pinMode(BUTTON_1_LED_PIN, OUTPUT);
//  pinMode(BUTTON_0_PIN, INPUT_PULLUP); 
//  pinMode(BUTTON_1_PIN, INPUT_PULLUP); 

  // Blink the LED for some fun to create a delay before loop() begins
//  for ( int i = 0; i < 3; i++ ) {
//    
//    digitalWrite(LED, HIGH);
//  	delay(100+i*100);
//  	digitalWrite(LED, LOW);
//  	delay(100+i*100);
//  }
//  digitalWrite(BUTTON_0_LED_PIN, LOW);  // unused currently, set to LOW anyway
//  digitalWrite(BUTTON_1_LED_PIN, LOW);  // unused currently, set to LOW anyway
  
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

  // must call so LED is updated w/ time
  loop_led();

  delay(10);  // prevent us from spending all the time inside loop() so interrupt based things can happen
}

void relay_setup() {
	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
  r0->off();
  r1->off();
}

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

// Blink the LED for a period based on time
static unsigned long led_blink_end = 0;
#define BLINK_DURATION_MS 500

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

void blink_led() {
  led_blink_end = millis() + BLINK_DURATION_MS;
}
