/*#ifndef Lights_h
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
//    unsigned long last_light_update;
    
    // Abstracting Neopixel Library
    void setPixelColor(uint16_t n, uint32_t c);
    void show();

  public:
    Lights( int, int, int );
//    void test();
    void loop();
    void off();
    void on();
//    void shoot( int direction );
    void setColors(uint32_t c1, uint32_t c2); 
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
};

#endif
*/
