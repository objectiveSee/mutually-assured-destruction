#include <Arduino.h>

#include "Button.h"

Button::Button( int pin_num )
{
  pin = pin_num;
}

bool Button::isOn() {
  return digitalRead(pin);
}