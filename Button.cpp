#include <Arduino.h>

#include "Button.h"

#define DEBOUNCE_DURATION 40

Button::Button( int pin_num )
{
  pin = pin_num;
  debounced_state = false;
  last_state = false;
  last_state_change = 0;
}

bool Button::isOn() {
  return debounced_state;
}

void Button::loop() {

	// debounce the button

	unsigned long current_time = millis();
	bool state_now = digitalRead(pin);

	if ( last_state != state_now ) {
		last_state_change = current_time;
	}

	if ( current_time - last_state_change > DEBOUNCE_DURATION ) {
		debounced_state = state_now;
	}

	last_state = state_now;	
}