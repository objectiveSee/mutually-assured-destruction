
#include <Arduino.h>
#include <IRLib.h>

#include "Relay.h"
#include "Lights.h"
#include "RemoteControl.h"

static Relay * r0 = 0;
static Relay * r1 = 0;
static Lights * lights = 0;
static RemoteControl * remote = 0;

void relay_loop();
void relay_setup();

#define LED 13
#define RELAY_0_PIN 10
#define RELAY_1_PIN 9
#define NEOPIXEL_PIN 8
#define NEOPIXEL_COUNT 12


void setup() {
  // put your setup code here, to run once:

	Serial.begin(9600); 
	
	relay_setup();

	digitalWrite(LED, HIGH);
	delay(2500);
	digitalWrite(LED, LOW);
	delay(2500);

	lights = new Lights(NEOPIXEL_PIN, NEOPIXEL_COUNT);

        remote = new RemoteControl(-1);  // note pin is specified in class
        
}

void loop() {

  // put your main code here, to run repeatedly:
//  relay_test();
  
  remote->loop();
  
  if ( remote->last_command == RemoteCommandStop ) {
    
    r0->off();
    r1->off();
    lights->off();
    
  } else if ( remote->last_command == RemoteCommandStart ) {
    
  }

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

#define INTERVAL 2000

void rapid_fire (unsigned long delayTime) {

	for ( int i = 0; i < 1; i++ ) {
		r0->on();
	  	r1->off();
	  	delay(delayTime);
		r0->off();
	  	r1->on();
	  	delay(delayTime);
	}
}

void relay_test () {
	unsigned long second = millis()/1000;
	if ( second % 3 ) {
		r0->on();
		r1->on();
	} else {
		r0->off();
		r1->off();		
	}
}

void relay_loop() {

	static unsigned long d = 100;

	rapid_fire(d);

	d+= 100;
	if ( d > 700 ) {
		d = 100;
	}

}

void relay_setup() {

	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);
}
