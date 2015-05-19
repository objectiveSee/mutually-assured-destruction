
#include "Accelerometer.h"
#include <Wire.h>
#include "Arduino.h"

// #include <stdlib.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// #define MOUNTING_AXIS_PRIMARY 0
// #define MOUNTING_AXIS_SECONDARY 0

#define ACCELEROMETER_THRESHOLD_POSITIVE 0.45f
#define ACCELEROMETER_THRESHOLD_NEGATIVE -0.45f
#define ACCELEROMETER_AT_TOP_DURATION 200 // in ms

// C function declarations. Implementation below.
float averageMeasuresCalc(float * measures );
AccelerometerPosition positionForValue(float value);
void printPosition(AccelerometerPosition position, bool newLine);

// Statics
Adafruit_MMA8451 mma = Adafruit_MMA8451();

#pragma mark - Accelerometer Class

Accelerometer::Accelerometer()
{
  for ( uint8_t i = 0; i < ACCELEROMETER_COUNT_MESASURES; i++ ) {
    last_measures[i] = 0;
  }
  position = AccelerometerPositionNone;
  last_position = AccelerometerPositionNone;
  last_position_change = 0;
  last_measures_index = 0;
  last_sample = -99999;
  last_sample_with_same_position = 0;
  position_changed = false;
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

void Accelerometer::loop()
{

  double timeNow = millis();
  bool changed = false;
  if ( timeNow - last_sample < ACCELEROMETER_SAMPLE_INTERVAL ) {
    Serial.println("Skipping Sample");
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

  // update variables
  average_measures = averageMeasuresCalc(&last_measures[0]);

  AccelerometerPosition currentPosition = positionForValue(average_measures);

  if ( position == currentPosition ) {

    last_sample_with_same_position = timeNow;

    Serial.print("Position is "); printPosition(currentPosition,1);

  } else {

    double timeSinceLastSame = timeNow - last_sample_with_same_position;
    if ( timeSinceLastSame > ACCELEROMETER_AT_TOP_DURATION ) {

      last_position = position;
      position = currentPosition;

      Serial.print("Position changed to ");
      printPosition(currentPosition,0);
      Serial.println("!!");

      changed = true;

    } else {

      Serial.print("Position changing to ");
      printPosition(currentPosition,1);
    }
  }

  position_changed = changed;

  // we all log
  // log();
}

float Accelerometer::angle_position()
{
  return average_measures;
}

void Accelerometer::log() {
  
  float angle = angle_position();
  float fangle = fabs(angle);

  // if ( fangle < .1 ) {
  //   print("Sides are level");
  // } else {}

  if ( angle > 0 ) {
    Serial.print("Side 0 at ");    
  } else {
    Serial.print("Side 1 at ");
  }
  Serial.print(fangle*100);
  Serial.println(" percent height");
}

#pragma mark - C helpers

AccelerometerPosition positionForValue(float value) {
  if ( value > ACCELEROMETER_THRESHOLD_POSITIVE ) {
    return AccelerometerPositionSide0Top;
  } else if ( value < ACCELEROMETER_THRESHOLD_NEGATIVE ) {
    return AccelerometerPositionSide1Top;
  }
  return AccelerometerPositionNone;
}

void printPosition(AccelerometerPosition position, bool newLine) {
  switch(position) {
    case AccelerometerPositionSide0Top:
      Serial.print("Side 0 Top");
      break;
    case AccelerometerPositionSide1Top:
      Serial.print("Side 1 Top");
      break;
    default:
      Serial.print("None");
  }
  if ( newLine ) {
    Serial.print("\n");
  }
}

float averageMeasuresCalc(float * measures ) {
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
