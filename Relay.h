#ifndef Relay_h
#define Relay_h

class Relay
{
  private:
    bool run;  // remember if we're running or not
    int pin;  // what pin is the heater relay on
    unsigned long last_changed;

  public:
    Relay( int );
    void on();
    void off();
    bool running();
    void loop();

    unsigned char * pattern_current;
    unsigned int pattern_length;
    unsigned long pattern_time_start;

    void setOnWithPattern(const unsigned char * pattern);
    void clearPattern();
    unsigned char patternCharAtIndex(const unsigned char * progmem_addr, unsigned int index);
};

#endif
