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
//#include "Button.h"

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
#define MAD_LOGGING 1
#define MAD_LIGHTS_ENABLED 1

// Static Members
static Relay * r0 = 0;
static Relay * r1 = 0;
//static Button * button0 = 0;
//static Button * button1 = 0;
static Lights * lights = 0;
static RemoteControl * remote = 0;
static Accelerometer * accelerometer = 0;
static boolean stopped = 0;
static boolean accel_enabled = false;

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

#if MAD_LOGGING
  Serial.println("It's a MAD World!");
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
  remote->loop();
//  button0->loop();
//  button1->loop();
  
#ifdef MAD_LOGGING
static int t = 0;
if ( t == 0 ) {
  Serial.println("loop");
}
t = (t+1)%1000;
#endif
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
      Serial.println("Relay 0 on");
#endif
      r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      
    } else if ( remote->last_command == RemoteCommandRight ) {
   
#if MAD_LOGGING
      Serial.println("Relay 1 on");
#endif
      r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);

    } else if ( remote->last_command == RemoteCommandBoth ) {
   
#if MAD_LOGGING
      Serial.println("Both Relays OnForDuration");
#endif
      r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);
      
    } else if ( remote->last_command == RemoteCommandToggleAccelerometer ) {
      accel_enabled = !accel_enabled;
      
    } else {
      // only do accelerometer based stuff if the remote isn't active (?)

      if ( accelerometer->position_changed) {
        if ( accelerometer->last_position == AccelerometerPositionNone ) {
          if ( accelerometer->position == AccelerometerPositionSide0Top ) {
            if ( accel_enabled ) {
              r0->setOnForDuration(FIRE_BURST_SHORT_DURATION);
            }
            toggleLightColors(true);
          } else if ( accelerometer->position == AccelerometerPositionSide1Top ) {
            if ( accel_enabled ) {
              r1->setOnForDuration(FIRE_BURST_SHORT_DURATION);
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

  delay(10);
}


void toggleLightColors (boolean side) { 
  
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
}


void relay_setup() {
	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
  r0->off();
  r1->off();
}
