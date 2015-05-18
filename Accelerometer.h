#ifndef __Accelerometer__
#define __Accelerometer__

// typedef enum  { 
//   AccelerometerPositionNone = 0,
//   AccelerometerPositionSide0Up,
//   AccelerometerPosition
// } AccelerometerPosition;

#define ACCELEROMETER_COUNT_MESASURES 8
#define ACCELEROMETER_SAMPLE_INTERVAL 25   // in ms

class Accelerometer
{
public:
    Accelerometer();
    void setup();
    void loop();
    float angle_position();
    void log();
//    ~Accelerometer();   
private:
    float last_measures[ACCELEROMETER_COUNT_MESASURES];
    int last_measures_index;
    bool angle_calculated;
    float angle;
    double last_sample;
};

#endif /* defined(__Accelerometer__) */
