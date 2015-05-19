#ifndef Relay_h
#define Relay_h

class Relay
{
  private:
    bool run;  // remember if we're running or not
    int pin;  // what pin is the heater relay on
    unsigned long last_changed;
    unsigned long duration_on;

  public:
    Relay( int );
    void on();
    void off();
    bool running();
    void loop();
    unsigned long time_running();
    
    void setOnForDuration( unsigned long );
};

#endif
