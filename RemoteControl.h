/**
#ifndef RemoteControl_h
#define RemoteControl_h

typedef enum  {
  RemoteCommandNone = 0,
  RemoteCommandStop,
  RemoteCommandStart,
  RemoteCommandSetup,
  RemoteCommandLeft,
  RemoteCommandRight,
  RemoteCommandBoth,
  RemoteCommandUp,
  RemoteCommandDown,
  RemoteCommandToggleAccelerometer, // undefined
  // RemoteCommandAdjustAccelerometerLeft,
  // RemoteCommandAdjustAccelerometerRight,
  RemoteCommandDigit1,
  RemoteCommandDigit2,
  RemoteCommandDigit3,
  RemoteCommandDigit4,
  RemoteCommandDigit5,
  RemoteCommandDigit6,
  RemoteCommandDigit7,
  RemoteCommandDigit8,
  RemoteCommandDigit9
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

    RemoteCommand last_commands[2];
    bool newCommand;

    RemoteControl( int );
    RemoteCommand loop();
    // RemoteCommand last_command();
    void clearCommand();
    // void off();
    // int running();
    // unsigned long time_running();
};

#endif
*/
