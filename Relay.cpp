#include <Arduino.h>
#include "Relay.h"

// Relay is active low, so you need a 0 to turn it on. 
#define RELAY_ON 0
#define RELAY_OFF 1

Relay::Relay( int whatPin )
{
  // initialize variables
  pin = whatPin;
  run = 0;
  last_changed = millis();


  // initialize physical objects
  pinMode( pin, OUTPUT );

  // don't forget that we don't know the state of the pin
  // so give it one
  digitalWrite( pin, RELAY_OFF );
}

void Relay::on()
{
  if( ! run )
  {
    run = 1;
    last_changed = millis();
  }
  digitalWrite( pin, RELAY_ON );

}

void Relay::off()
{
  if ( run ) {
    run = 0;
    last_changed = millis();
  }
  digitalWrite( pin, RELAY_OFF );
}

unsigned long Relay::time_running() {
  if ( run ) {
    return millis() - last_changed;
  }
  return 0;
}

int Relay::running()
{
  return run;

}
