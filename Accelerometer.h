#ifndef __Accelerometer__
#define __Accelerometer__

typedef enum  { 
  AccelerometerPositionNone = 0,
  AccelerometerPositionSide0Top,
  AccelerometerPositionSide1Top
} AccelerometerPosition;

#define ACCELEROMETER_COUNT_MESASURES 4
#define ACCELEROMETER_SAMPLE_INTERVAL 10   // in ms

class Accelerometer
{
public:
    Accelerometer();
    void setup();
    void loop();
    float angle_position();
    void log();
    AccelerometerPosition position;
    AccelerometerPosition last_position;
    bool position_changed;  // set for 1 cycle of loop() only. Better check it fool!

//    ~Accelerometer();   

private:
    float last_measures[ACCELEROMETER_COUNT_MESASURES];
    float average_measures;
    int last_measures_index;
    float angle;
    double last_sample;
    double last_position_change;
    double last_sample_with_same_position;
};

#endif /* defined(__Accelerometer__) */
