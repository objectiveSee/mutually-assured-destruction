
#include "Accelerometer.h"
#include <Wire.h>
#include "Arduino.h"
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "build.h"
#include "Settings.h"

/*
 * Amount of time seesaw must be at a position for it to be registered. Sort of like debouncing a button.
 */
#define ACCELEROMETER_AT_TOP_DURATION 60 // in ms

/*
 * Function Declarations
 */
float averageMeasuresCalc(float * measures );
AccelerometerPosition positionForValue(float value);
void printPosition(AccelerometerPosition position, bool newLine);
void logAdjustment(int side, float value);

/*
 * Static Memory
 */
Adafruit_MMA8451 mma = Adafruit_MMA8451();

/*
 * Accelerometer Class Implementation
 */
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
  working = false;
}

void Accelerometer::setup()
{

  if (! mma.begin()) {
    // always log this because it's so crucial
    Serial.println(F("Accelerometer error"));
    return;
  }
  working = true;

  mma.setRange(MMA8451_RANGE_2_G);

#if MAD_ACCELEROMETER_LOGGING
  Serial.println(F("MMA8451 found!"));
  Serial.print(F("Range = ")); Serial.print(2 << mma.getRange());
  Serial.println(F("G"));
#endif
}

void Accelerometer::loop()
{
  if ( !working ) {
    return;
  }

  unsigned long timeNow = millis();
  bool changed = false;
  if ( timeNow - last_sample < ACCELEROMETER_SAMPLE_INTERVAL ) {
#if MAD_ACCELEROMETER_LOGGING
//    Serial.println("Skipping Sample");
#endif
    return;
  }

  last_sample = timeNow;

  mma.read();

    // Get a new sensor event
  sensors_event_t event;
  mma.getEvent(&event);

  float acceleration = event.acceleration.x;
  #if MAD_ACCELEROMETER_LOGGING
  Serial.print(F("X=\t")); Serial.println(acceleration);
#endif

  last_measures_index = (last_measures_index+1)%ACCELEROMETER_COUNT_MESASURES;
  last_measures[last_measures_index] = acceleration;

  // update variables
  average_measures = averageMeasuresCalc(&last_measures[0]);

  AccelerometerPosition currentPosition = positionForValue(average_measures);

  if ( position == currentPosition ) {

    last_sample_with_same_position = timeNow;
#if MAD_ACCELEROMETER_LOGGING
//    Serial.print("Position is "); printPosition(currentPosition,1);
#endif
  } else {

    unsigned long timeSinceLastSame = timeNow - last_sample_with_same_position;
    if ( timeSinceLastSame > ACCELEROMETER_AT_TOP_DURATION ) {

      last_position = position;
      position = currentPosition;
#if MAD_ACCELEROMETER_LOGGING
      Serial.print(F("Position changed to "));
      printPosition(currentPosition,0);
      Serial.println("!!");
#endif

      changed = true;

    } else {
#if MAD_ACCELEROMETER_LOGGING
      Serial.print(F("Position changing to "));
      printPosition(currentPosition,1);
#endif
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

#if MAD_ACCELEROMETER_LOGGING

  float angle = angle_position();
  float fangle = fabs(angle);

  if ( angle > 0 ) {
    Serial.print(F("Side 0 at "));
  } else {
    Serial.print(F("Side 1 at "));
  }
  Serial.print(fangle*100);
  Serial.println(F(" percent height"));
#endif

}

#define MAD_ACCELEROMETER_ADJUST_AMOUNT 0.01f

void Accelerometer::storeCurrentAngleForSide() {

  float angle = angle_position();

  // use sign on the angle to determine which side to set the angle for
  if ( angle > 0  ) {
    my_settings.accelerometer_angle_positive = angle;
  } else {
    my_settings.accelerometer_angle_negative= angle;
  }
  if ( angle < 0.1 && angle > -0.1 ) {
    Serial.println(F("WARNING: Setting trigger angles to a potentially poor value"));
  }
  
  #if MAD_ACCELEROMETER_LOGGING
  Serial.print("Updating trigger angles to: ");
  Serial.print("Positive: "); Serial.print(my_settings.accelerometer_angle_positive);
  Serial.print("Negative: "); Serial.println(my_settings.accelerometer_angle_negative);
  #endif
  
  saveConfig();  
};

/*
 * C helpers
 */
void logAdjustment(int side, float value) {
#if MAD_ACCELEROMETER_LOGGING
  Serial.print(F("Adjustment: Side ")); Serial.print(side); Serial.print(F(" = ")); Serial.println(value);
#endif
}

/**
 * Returns the `AccelerometerPosition` corresponding to the given input angle.
 */
AccelerometerPosition positionForValue(float value) {

  if ( value > my_settings.accelerometer_angle_positive ) {
    return AccelerometerPositionSide0Top;
  } else if ( value < my_settings.accelerometer_angle_negative) {
    return AccelerometerPositionSide1Top;
  }
  
  return AccelerometerPositionNone;
}

/*
 * Logs the position as a string and optionally prints a newline `\n`.
 */
void printPosition(AccelerometerPosition position, bool newLine) {

#if MAD_ACCELEROMETER_LOGGING

  switch(position) {
    case AccelerometerPositionSide0Top:
      Serial.print(F("Side 0 Top"));
      break;
    case AccelerometerPositionSide1Top:
      Serial.print(F("Side 1 Top"));
      break;
    default:
      Serial.print(F("None"));
  }
  if ( newLine ) {
    Serial.print(F("\n"));
  }

#endif

}

float averageMeasuresCalc(float * measures ) {
  float average = 0;
  for ( uint8_t i = 0; i < ACCELEROMETER_COUNT_MESASURES; i++ ) {
    average += measures[i];
  }
  return average / ACCELEROMETER_COUNT_MESASURES;
}
