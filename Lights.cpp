#include <Arduino.h>
#include "Lights.h"
#include "Adafruit_NeoPixel.h"

typedef uint32_t color;
Adafruit_NeoPixel *stripA;
Adafruit_NeoPixel *stripB;

#define MAD_LIGHTS_LOGGING 1


Lights::Lights( int pA, int pB, int countLights )
{
  // initialize variables
  pinA = pA;
  pinB = pB;
  numLights_total = countLights * 2;
  numLights_perStrip = countLights;

  stripA = new Adafruit_NeoPixel(numLights_perStrip, pinA, NEO_GRB + NEO_KHZ800);
//  stripB = new Adafruit_NeoPixel(numLights_perStrip, pinB, NEO_GRB + NEO_KHZ800);
  
  stripA->begin();
  //stripB->begin();
  
  isOn = 0; 

  // run = 0;
  // last_changed = millis();


  // // initialize physical objects
  // pinMode( pin, OUTPUT );

  // // don't forget that we don't know the state of the pin
  // // so give it one
  // digitalWrite( pin, Lights_OFF );
}

void Lights::loop() {   
}


void Lights::setPixelColor(uint16_t n, uint32_t c) {
  if ( n >= numLights_perStrip ) { 
    stripB->setPixelColor( n - numLights_perStrip, c);
  } else {
    stripA->setPixelColor( n, c);
  }
}

void Lights::show() { 
  if ( isOn ) {
    stripA->show();
    stripB->show();
  }
}

void Lights::test() {  
}

void Lights::on() { 
  isOn = 1;
}

void Lights::off()
{
  for ( int i = 0; i < numLights_total; i++ ) {
    color c = stripA->Color(0,0,0);
    setPixelColor(i, c);
  }
  isOn = 0;
  stripA->show();
  stripB->show();
}

void Lights::shoot( int direction ) {  
}

