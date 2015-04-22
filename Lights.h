#ifndef Lights_h
#define Lights_h

class Lights
{
  private:
    int pin;  // what pin is the heater Lights on
    int numLights;

  public:
    Lights( int , int);
    int test();
};

#endif
