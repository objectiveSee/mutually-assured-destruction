
#include <Arduino.h>
#include "Relay.h"

static Relay * r0;
static Relay * r1;

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

void relay_loop() {
  r0->on();
  r1->off();
  delay(1000);
  r0->off();
  r1->on();
  delay(1000);
}

void relay_setup() {

	r0 = new Relay(RELAY_0_PIN);
	r1 = new Relay(RELAY_1_PIN);

	pinMode(RELAY_0_PIN, OUTPUT);
	pinMode(RELAY_1_PIN, OUTPUT);

	digitalWrite(RELAY_0_PIN, 0);
	digitalWrite(RELAY_1_PIN, 0);
}
