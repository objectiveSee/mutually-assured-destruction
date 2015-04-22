
#include <Arduino.h>
#include "Relay.h"

static Relay * r0 = 0;
static Relay * r1 = 0;

void relay_loop();
void relay_setup();

#define LED 13
#define RELAY_0_PIN 10
#define RELAY_1_PIN 9


void setup() {
  // put your setup code here, to run once:
	relay_setup();

	digitalWrite(LED, HIGH);
	delay(2500);
	digitalWrite(LED, LOW);
	delay(2500);
}

void loop() {

  // put your main code here, to run repeatedly:
  relay_loop();
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
