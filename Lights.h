#ifndef Lights_h
#define Lights_h

//class Adafruit_NeoPixel;  // forward declaration of a class

class Lights
{
  private:
    int pinA;
    int pinB;
    int numLights_perStrip;
    int numLights_total;
    int isOn;
    
    // Abstracting Neopixel Library
    void setPixelColor(uint16_t n, uint32_t c);
    void show();

  public:
    Lights( int, int, int );
    void test();
    void loop();
    void off();
    void on();
    void shoot( int direction );
};

#endif
