#include <Arduino.h>

#include "Button.h"

#define DEBOUNCE_DURATION 20
#define MAD_BUTTON_LOGGING 1

Button::Button( int pin_num )
{
  my_pin = pin_num;
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
	bool state_now = digitalRead(my_pin);

	if ( last_state != state_now ) {
		last_state_change = current_time;
	} else {
        	if ( (debounced_state != state_now)  && (current_time - last_state_change > DEBOUNCE_DURATION) ) {
		  debounced_state = state_now;
#if MAD_BUTTON_LOGGING
		  Serial.print("Button (pin "); Serial.print(my_pin); Serial.print(") is now "); Serial.println(debounced_state?"Pressed":"Released");
#endif
	       }
        }

	last_state = state_now;	
}

