#include <Arduino.h>
#include "Lights.h"
#include "Adafruit_NeoPixel.h"

typedef uint32_t color;
Adafruit_NeoPixel *stripA;
//Adafruit_NeoPixel *stripB;

#define MAD_LIGHTS_LOGGING 1

#define LIGHT_UPDATE_INTERVAL 500


Lights::Lights( int pA, int pB, int countLights )
{
  // initialize variables
  pinA = pA;
  pinB = pB;
  numLights_total = countLights;
  numLights_perStrip = countLights;

  stripA = new Adafruit_NeoPixel(numLights_perStrip, pinA, NEO_GRB + NEO_KHZ800);
//  stripB = new Adafruit_NeoPixel(numLights_perStrip, pinB, NEO_GRB + NEO_KHZ800);
  
  stripA->begin();
//  stripB->begin();
  
  isOn = 0; 
}

void Lights::setColors(uint32_t c1, uint32_t c2) {
 
  if ( ! isOn ) {
    return;
  }
  
  for ( int i = 0; i < numLights_perStrip; i++ ) {
   // if ( i % 3 == 0 ) {
    stripA->setPixelColor(i, c1);
//    stripB->setPixelColor(i, c2);
    //} else {
      
  }
  show();  
} 

void Lights::loop() { 
    
//  static int looper = 0;
//  static int r = 40;
//  static int b = 0;
//  static boolean flop = false;
//  
//  unsigned long sss = millis();
//  if ( sss - last_light_update > 
//  
//  for ( int i = 0; i < numLights_perStrip; i++ ) {
//    
//    looper++;
//    if ( i == looper ) {
//        color c = stripA->Color(0,0,40);
//        stripA->setPixelColor(i, c);
//    } else {
//        color c = stripA->Color(40,0,0);
//        stripA->setPixelColor(i, c);      
//    }
// }  
//  stripA->show();
//  
//#if MAD_LIGHTS_LOGGING
//  unsigned long eee = millis() - sss;
//  if ( eee >= 2 ) {
//    Serial.print("Lights took "); Serial.println(eee);
//  }
//#endif
}


void Lights::setPixelColor(uint16_t n, uint32_t c) {
//  if ( n >= numLights_perStrip ) { 
//    stripB->setPixelColor( n - numLights_perStrip, c);
//  } else {
    stripA->setPixelColor( n, c);
//  }
}

void Lights::show() { 
  if ( isOn ) {
    stripA->show();
//    stripB->show();
  }
}

//void Lights::test() {  
//}

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
//  stripB->show();
}

uint32_t Lights::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

