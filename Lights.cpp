#include <Arduino.h>
#include "Lights.h"
#include "Adafruit_NeoPixel.h"

typedef uint32_t color;
Adafruit_NeoPixel *strip ;

Lights::Lights( int whatPin, int countLights )
{
  // initialize variables
  pin = whatPin;
  numLights = countLights;

  strip = new Adafruit_NeoPixel(60, whatPin, NEO_GRB + NEO_KHZ800);

  strip->begin();

  for ( int i = 0; i < numLights; i++ ) {
    color c = strip->Color(10, 0,0);
    strip->setPixelColor(i, c);
  }
  strip->show();

  // run = 0;
  // last_changed = millis();


  // // initialize physical objects
  // pinMode( pin, OUTPUT );

  // // don't forget that we don't know the state of the pin
  // // so give it one
  // digitalWrite( pin, Lights_OFF );
}
//unsigned long millis
void Lights::loop()
{
  unsigned long second = millis()/1000;

  for ( int i = 0; i < numLights; i++ ) {

    Serial.println(second);
    color c = strip->Color(11, (second%2)?11:0,0);
    strip->setPixelColor(i, c);
  }
  strip->show();
  if ( strip ) {
    Serial.println("Looping too!!\n");    
  }
}

void Lights::off()
{
  for ( int i = 0; i < numLights; i++ ) {
    color c = strip->Color(0,0,0);
    strip->setPixelColor(i, c);
  }
  strip->show();
}
  

int Lights::test()
{
  return pin;

}
