#ifndef Relay_h
#define Relay_h

class Relay
{
  private:
    bool run;  // remember if we're running or not
    int pin;  // what pin is the heater relay on
    unsigned long last_changed;
    void on();  // private, use setOnWithPattern() instead
    void off(); // private, use clearPattern() instead
    unsigned char patternCharAtIndex(const unsigned char * progmem_addr, unsigned int index);

  public:
    Relay( int );
    bool running();   // indicates whether the relay is on or off.
    void loop();

    unsigned char * pattern_current;
    unsigned int pattern_length;
    unsigned long pattern_time_start;

    void setOnWithPattern(const unsigned char * pattern);
    void clearPattern();
};

#endif
