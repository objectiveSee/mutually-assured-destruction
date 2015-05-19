// Arduino Libraries
#include <Arduino.h>
#include <IRLib.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// MAD Libraries
#include "Accelerometer.h"
#include "Relay.h"
#include "Lights.h"
#include "RemoteControl.h"

// Static Members
static Relay * r0 = 0;
static Relay * r1 = 0;
static Lights * lights = 0;
static RemoteControl * remote = 0;
static Accelerometer * accelerometer = 0;
static boolean stopped = 0;

// Function Declerations
void relay_setup();

// Defines
#define LED 13
#define RELAY_0_PIN 10
#define RELAY_1_PIN 9
#define NEOPIXEL_PIN_A 8
#define NEOPIXEL_PIN_B -1
#define NEOPIXEL_COUNT 12
#define FIRE_BURST_SHORT_DURATION 200

#define MAD_LOGGING 1


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

#if MAD_LOGGING
  Serial.println("It's a MAD World!");
#endif

  accelerometer = new Accelerometer();
  accelerometer->setup();

	lights = new Lights(NEOPIXEL_PIN_A, NEOPIXEL_PIN_B, NEOPIXEL_COUNT);
  lights->on();

  remote = new RemoteControl(-1);  // note pin is specified in class        
}

void loop() {
  
  accelerometer->loop();
  // accelerometer->log();

  remote->loop();

  // indicates whether we should clear last command
  // boolean clearLastCommand = true;

  // Process remote command:

  if ( remote->last_command == RemoteCommandStop ) {
    
    stopped = 1;
    r0->off();
    r1->off();
    lights->off();
    
  } else if ( remote->last_command == RemoteCommandStart ) {  
 
     stopped = 0;
     lights->on();
    
  }
  
  if (!stopped) {

    if ( remote->last_command == RemoteCommandLeft ) {
#if MAD_LOGGING
      Serial.println("Relay 0 OnForDuration");
#endif
      r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      
    } else if ( remote->last_command == RemoteCommandRight ) {
   
#if MAD_LOGGING
      Serial.println("Relay 1 OnForDuration");
#endif
      r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);

    } else if ( remote->last_command == RemoteCommandBoth ) {
   
#if MAD_LOGGING
      Serial.println("Both Relays OnForDuration");
#endif
      r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      
    } else {
      // only do accelerometer based stuff if the remote isn't active (?)

      if ( accelerometer->position_changed ) {
        if ( accelerometer->last_position == AccelerometerPositionNone ) {
          if ( accelerometer->position == AccelerometerPositionSide0Top ) {
            r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
          } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {
            r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);
          }
        }
      }
    }

  }   /** end code that only runs if !stopped **/
  
  r0->loop();
  r1->loop();
  lights->loop();
  
  // if ( clearLastCommand ) {
   // clear last command 
   remote->clearCommand();
  // }

  // blink every second
  // unsigned long second = (unsigned long)((float)millis() / 1000);
  // if ( second % 2) {
	// digitalWrite(LED, HIGH); 	
  // } else {
 	// digitalWrite(LED, LOW);
  // }
  // Serial.println("Second is");
  // Serial.println);
//  lights->loop();
//  delay(100);
}

void relay_setup() {
	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
  r0->off();
  r1->off();
}
