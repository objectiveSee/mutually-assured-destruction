#include <Arduino.h>
#include "Relay.h"

// Relay is active low, so you need a 0 to turn it on.
#define RELAY_ON 0
#define RELAY_OFF 1
#define MAX_TIME_ON 1000
#define MAD_RELAY_LOGGING 1

#define PATTERN_INTERVAL_MULTIPLIER (int)10


// Class Begin
Relay::Relay( int whatPin )
{
  // initialize variables
  pin = whatPin;
  run = false;
  last_changed = millis();
  pattern_current = 0;


  // initialize physical objects
  pinMode( pin, OUTPUT );

  // don't forget that we don't know the state of the pin
  // so give it one
  digitalWrite( pin, RELAY_OFF );

  off();
}

void Relay::on()
{
  if( ! run )
  {
    run = true;
    last_changed = millis();
  }
  digitalWrite( pin, RELAY_ON );

}

void Relay::clearPattern() {
  pattern_current = 0;
  pattern_length = 0;
  pattern_time_start = 0;
  off();
}

void Relay::off()
{
  if ( run ) {
    run = false;
    last_changed = millis();
  }
  digitalWrite( pin, RELAY_OFF );
}

bool Relay::running()
{
  return run;
}

#pragma mark - Should be in Subclass for Fire Relay

void Relay::loop() {

  unsigned long now = millis();

  if ( pattern_current ) {

    bool isOn = false;
    bool matched = false;
    unsigned long timeSincePatternStart = now - pattern_time_start;
    unsigned long patternTime = 0;

    // Pattern alternatates durations of on and off, starting with off.
    // for example : 100,200,300,400 = On (100 ms), Off(200 ms), etc

    // todo when pattern finishes, turn off and set pattern_current to nil

    for ( unsigned char i = 0; i < pattern_length; i++ ) {

      unsigned char character = patternCharAtIndex(pattern_current, i);

      patternTime += ( character * PATTERN_INTERVAL_MULTIPLIER );

      if ( timeSincePatternStart < patternTime ) {
        // we are inside this interval of the pattern
        isOn = ((i%2) == 1);
        matched = true;
        // Serial.print("matched on "); Serial.println(i);
        break;
      }

    }

    if ( !matched ) {
      // isOn will be false
      // reset the pattern
      Serial.println(F("Pattern done"));
      clearPattern();
    }

    if ( isOn ) {
      on();
    } else {
      off();
    }

  } else {
    off();
  }
}

unsigned char Relay::patternCharAtIndex(const unsigned char * progmem_addr, unsigned int index) {
  unsigned char myChar =  (unsigned char)pgm_read_byte_near(progmem_addr + index);
  // Serial.println(myChar, HEX);
  return myChar;
}

void Relay::setOnWithPattern(const unsigned char * pattern) {

  if ( !pattern ) {
    return;
  }
  pattern_current = (unsigned char *)pattern; // cast from `const`
  pattern_time_start = millis();

  for ( pattern_length = 0; pattern_length <= 1000; pattern_length++ ) {
    if ( patternCharAtIndex(pattern_current, pattern_length) == 255 ) {
      break;
    }
  }

  Serial.print(F("Pattern Length is ")); Serial.println(pattern_length);
};
