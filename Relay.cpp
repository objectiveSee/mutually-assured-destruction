#include <Arduino.h>
#include "Relay.h"

// Relay is active low, so you need a 0 to turn it on.
#define RELAY_ON 0
#define RELAY_OFF 1
#define MAX_TIME_ON 1000
#define MAD_RELAY_LOGGING 0

#define PATTERN_INTERVAL_MULTIPLIER (int)10


// Static Memory (Patterns)
// Rapid Bursts
#define RAPID_ON_DURATION 10
#define RAPID_OFF_DURATION 10
unsigned char RAPID_BURST_A[11] = {0,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,255};
unsigned char RAPID_BURST_B[11] = {RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,RAPID_OFF_DURATION,RAPID_ON_DURATION,255};

// L/R/Middle
unsigned char LRM_A[5] = {0,80,40,80,255};
unsigned char LRM_B[5] = {40,80,40,80,255};

// Single Button Press Pattern
unsigned char SINGLE_BUTTON_PRESS[3] = {0,30,255};


// unsigned char RAPID_BURST_A[12] = {0,40,40,40,40,40,40,20,20,20,20,20,255};
// unsigned char RAPID_BURST_B[2] = {0,255};


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

void Relay::off()
{
  if ( run ) {
    run = false;
    last_changed = millis();
  }

  pattern_current = 0;
  pattern_length = 0;
  pattern_time_start = 0;

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

      patternTime += ( pattern_current[i] * PATTERN_INTERVAL_MULTIPLIER );

      if ( timeSincePatternStart < patternTime ) {
        // we are inside this interval of the pattern
        isOn = ((i%2) == 1);
        matched = true;
        Serial.print("matched on "); Serial.println(i);
        break;
      }

    }

    if ( !matched ) {
      // isOn will be false
      // reset the pattern
      Serial.println("Pattern done");
      pattern_current = 0;
      pattern_length = 0;
      pattern_time_start = 0;
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

void Relay::setOnWithPattern(unsigned char * pattern) {

  if ( !pattern ) {
    return;
  }
  pattern_current = pattern;
  pattern_time_start = millis();

  for ( pattern_length = 0; pattern_length <= 255; pattern_length++ ) {
    if ( pattern[pattern_length] == 255 ) {
      break;
    }
  }

  Serial.print("Pattern Length is "); Serial.println(pattern_length);
};
