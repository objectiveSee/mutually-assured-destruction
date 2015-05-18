
#include "Accelerometer.h"
#include <Wire.h>
#include "Arduino.h"

// #include <stdlib.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// #define MOUNTING_AXIS_PRIMARY 0
// #define MOUNTING_AXIS_SECONDARY 0

float averageMeasures(float * measures );

Adafruit_MMA8451 mma = Adafruit_MMA8451();

Accelerometer::Accelerometer()
{
  for ( uint8_t i = 0; i < ACCELEROMETER_COUNT_MESASURES; i++ ) {
    last_measures[i] = 0;
  }
  last_measures_index = 0;
  angle_calculated = true;
  last_sample = -99999;
}

void Accelerometer::setup()
{

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G"); 
}

float Accelerometer::angle_position()
{
  if ( !angle_calculated ) {
    angle_calculated = true;
    angle = averageMeasures(&last_measures[0]);
  }
  return angle;
}

void Accelerometer::log() {
  
  float angle = angle_position();
  float fangle = fabs(angle);

  // if ( fangle < .1 ) {
  //   print("Sides are level");
  // } else {}

  if ( angle > 0 ) {
    Serial.print("Side 0 at ");    
  } else if ( angle < 0 ) {
    Serial.print("Side 1 at ");
  }
  Serial.print(fangle*100);
  Serial.println(" percent height");
}

void Accelerometer::loop()
{

  double timeNow = millis();
  if ( timeNow - last_sample < ACCELEROMETER_SAMPLE_INTERVAL ) {
    return;
  }

  last_sample = timeNow;

  mma.read();

    // Get a new sensor event
  sensors_event_t event; 
  mma.getEvent(&event);

  float acceleration = event.acceleration.x;
  // Serial.print("X=\t"); Serial.println(acceleration); 

  last_measures_index = (last_measures_index+1)%ACCELEROMETER_COUNT_MESASURES;
  last_measures[last_measures_index] = acceleration;

  angle_calculated = false;


  /*
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();



  // Display the results (acceleration is measured in m/s^2)
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  */

  log();
}


#pragma mark - C helpers

float averageMeasures(float * measures ) {
  float average = 0;
  for ( uint8_t i = 0; i < ACCELEROMETER_COUNT_MESASURES; i++ ) {
    average += measures[i];
  }
  return average / ACCELEROMETER_COUNT_MESASURES;
}

// void accelerometer_log() {

//     // Read the 'raw' data in 14-bit counts
//   mma.read();
// //  Serial.print("X:\t"); Serial.print(mma.x); 
// //  Serial.print("\tY:\t"); Serial.print(mma.y); 
// //  Serial.print("Z: "); Serial.print(mma.z);  Serial.print("\tEvent.accel.z=\t");
// //  Serial.println();

//   /* Get a new sensor event */ 
//   sensors_event_t event; 
//   mma.getEvent(&event);

//   /* Display the results (acceleration is measured in m/s^2) */
//   Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
//   Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
//   Serial.print(event.acceleration.z); Serial.print("\t");
//   Serial.println("m/s^2 ");  
// }
