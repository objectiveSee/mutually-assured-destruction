#ifndef Lights_h
#define Lights_h

// class Adafruit_NeoPixel;  // forward declaration of a class

class Lights
{
  private:
    int pin;  // what pin is the heater Lights on
    int numLights;

  public:
    Lights( int , int);
    int test();
    void loop();
};

#endif
