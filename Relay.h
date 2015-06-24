#ifndef Relay_h
#define Relay_h

extern unsigned char RAPID_BURST_A[];
extern unsigned char RAPID_BURST_B[];
extern unsigned char LRM_A[];
extern unsigned char LRM_B[];
extern unsigned char SINGLE_BUTTON_PRESS[];

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
    unsigned char pattern_length;
    unsigned long pattern_time_start;

    void setOnWithPattern(unsigned char * pattern);
    void clearPattern();
};

#endif
