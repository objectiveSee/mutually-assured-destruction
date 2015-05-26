#include <Arduino.h>
#include "RemoteControl.h"
#include <IRLib.h>

IRrecv My_Receiver(7);   // TODO: use whatPin
IRdecode My_Decoder;

#define MAD_REMOTE_LOGGING  1

RemoteControl::RemoteControl( int whatPin )
{
  // initialize variables
  pin = whatPin;
  pin = 7;  // TODO: fix
  last_command = RemoteCommandNone;

  My_Receiver.enableIRIn(); // Start the receiver
}

RemoteCommand RemoteControl::loop() {

  RemoteCommand r =  RemoteCommandNone;
  
  if (My_Receiver.GetResults(&My_Decoder)) {
   
    My_Decoder.decode();		//Decode the data
    //My_Decoder.DumpResults();	//Show the results on serial monitor

   if (My_Decoder.decode_type== NEC) {
     
#if MAD_REMOTE_LOGGING
      Serial.print("Remote: ");  Serial.println(My_Decoder.value);
#endif

       switch(My_Decoder.value) {
         
        case 0xFD609F:  //Stop button
          r = RemoteCommandStop; 
          break;
          
        case 0xFD807F: // Play
          r = RemoteCommandToggleAccelerometer;
          break;
          
        case 0xFD20DF: // Setup
          r = RemoteCommandStart;
          break;
          
        case 0xFD10EF:
#if MAD_REMOTE_LOGGING
      Serial.println("Remote Left");
#endif
          r = RemoteCommandLeft;
          break;
          
        case 0xFD50AF:
#if MAD_REMOTE_LOGGING
      Serial.println("Remote Right");
#endif
           r = RemoteCommandRight;
          break;
          
        case 0xFD906F:
          r = RemoteCommandBoth;
          break;

        case 0xFFFFFFFF:  // repeat last
          r = last_command;
#if MAD_REMOTE_LOGGING
      Serial.println("Remote Repeat Last");
#endif
          break;
      }
     
   }
    My_Receiver.resume(); 		//Restart the receiver
  }
  
  last_command = r;
  
  return r;
}

void RemoteControl::clearCommand() {
  last_command = RemoteCommandNone;
}
