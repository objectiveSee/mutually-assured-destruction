#include <Arduino.h>
#include "RemoteControl.h"
#include <IRLib.h>

IRrecv My_Receiver(7);   // TODO: use whatPin
IRdecode My_Decoder;

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
     
       switch(My_Decoder.value) {
         
        case 0xFD609F:  //Stop button
          r = RemoteCommandStop; 
          break;
          
        case 0xFD807F: // Play
        case 0xFD20DF: // Setup
          r = RemoteCommandStart;
          break;
          
        case 0xFD10EF:
          r = RemoteCommandLeft;
          break;
          
        case 0xFD50AF:
           r = RemoteCommandRight;
          break;
          
        case 0xFD906F:
          r = RemoteCommandBoth;
          break;

        case 0xFFFFFFFF:  // repeat last
          r = last_command;
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

#pragma mark - Private












