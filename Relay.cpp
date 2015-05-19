#include <Arduino.h>
#include "Relay.h"

// Relay is active low, so you need a 0 to turn it on. 
#define RELAY_ON 0
#define RELAY_OFF 1
#define MAX_TIME_ON 1000

Relay::Relay( int whatPin )
{
  // initialize variables
  pin = whatPin;
  run = 0;
  last_changed = millis();
  duration_on = 0;


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
  duration_on = 0;
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

#pragma mark - Should be in Subclass for Fire Relay

void Relay::loop() {
  
  unsigned long now = millis();
  
  if ( run ) {
    
   Serial.print(time_running());
   Serial.println("ms running");
    
   if ( duration_on && now > last_changed + duration_on ) {
     
     Serial.println("Duration expired on Relay. Turning off now");
     off();
     
   } else if ( run && ( time_running() > MAX_TIME_ON ) ) {
     
     Serial.println("Relay left on for too long. Turning off now");
     off(); 

   } else {
     if ( duration_on ) {
       unsigned long time_remaining = last_changed + duration_on - now;
       Serial.print(time_remaining);
       Serial.println("ms remaining");
     }
   }
   
   
 } 
}

void Relay::setOnForDuration( unsigned long duration ) {
  
  on();
  duration_on = duration; 
 
 last_changed = millis();     // in case we were already on, we want a better value for last_changed 
  
  Serial.print("Relay on with a duration = ");
  Serial.print(duration);
  Serial.println(" ms"); 
}
