#include <Arduino.h>
#include "Lights.h"
#include "Adafruit_NeoPixel.h"

// Lights is active low, so you need a 0 to turn it on. 

Lights::Lights( int whatPin, int countLights )
{
  // initialize variables
  pin = whatPin;
  numLights = countLights;

  Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, whatPin, NEO_GRB + NEO_KHZ800);
  // run = 0;
  // last_changed = millis();


  // // initialize physical objects
  // pinMode( pin, OUTPUT );

  // // don't forget that we don't know the state of the pin
  // // so give it one
  // digitalWrite( pin, Lights_OFF );
}

int Lights::test()
{
  return pin;

}
