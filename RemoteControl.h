#ifndef RemoteControl_h
#define RemoteControl_h

typedef enum  { 
  RemoteCommandNone = 0,
  RemoteCommandStop,
  RemoteCommandStart,
  RemoteCommandLeft,
  RemoteCommandRight,
  RemoteCommandBoth,
  RemoteCommandUp,
  RemoteCommandDown
} RemoteCommand;

class RemoteControl
{
  private:
    // int run;  // remember if we're running or not
    int pin;  // what pin is the heater relay on
    // unsigned long last_changed;
//    unsigned long last_command;
//    void handleResult( unsigned long );

  public:
    RemoteCommand last_command;
    RemoteControl( int );
    RemoteCommand loop();
    void clearCommand();
    // void off();
    // int running();
    // unsigned long time_running();
};

#endif
