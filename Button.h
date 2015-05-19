#ifndef Button_h
#define Button_h

class Button
{
  private:
    int pin;
    
  public:
    Button( int );
    bool isOn();
};

#endif
