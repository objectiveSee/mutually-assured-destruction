#include <Arduino.h>
#include "Relay.h"

Relay::Relay( int whatPin )
{
  // initialize variables
  pin = whatPin;
  run = 0;


  // initialize physical objects
  pinMode( pin, OUTPUT );

  // don't forget that we don't know the state of the pin
  // so give it one
  digitalWrite( pin, LOW );
}

void Relay::on()
{
  if( ! run )
  {
    run = 1;
    digitalWrite( pin, HIGH );
    last_changed = millis();
  }

}

void Relay::off()
{
  if ( run ) {
        last_changed = millis();
  }
  run = 0;
  digitalWrite( pin, LOW);
}

int Relay::running()
{
  return run;

}
