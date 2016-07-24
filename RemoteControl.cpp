/**
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

  newCommand = false;
  last_commands[0] = RemoteCommandNone;
  last_commands[1] = RemoteCommandNone;

  My_Receiver.enableIRIn(); // Start the receiver
}

RemoteCommand RemoteControl::loop() {

  RemoteCommand r =  RemoteCommandNone;

  if (My_Receiver.GetResults(&My_Decoder)) {

    My_Decoder.decode();		//Decode the data
    //My_Decoder.DumpResults();	//Show the results on serial monitor

   if (My_Decoder.decode_type== NEC) {

// #if MAD_REMOTE_LOGGING
//       Serial.print("Remote: ");  Serial.println(My_Decoder.value);
// #endif

       switch(My_Decoder.value) {

        case 0xFD609F:  //Stop button
          r = RemoteCommandStop;
          break;

        case 0xFD807F: // Play
          r = RemoteCommandStart;
          break;

        case 0xFD20DF: // Setup
          r = RemoteCommandSetup;
          break;

        case 0xFDA05F:
#if MAD_REMOTE_LOGGING
      Serial.println("Remote Up");
#endif
          r = RemoteCommandUp;
          break;
        case 0xFDB04F:
#if MAD_REMOTE_LOGGING
      Serial.println("Remote Down");
#endif
          r = RemoteCommandDown;
          break;

//         case 0x00DEAD: // TODO pick real value
// #if MAD_REMOTE_LOGGING
//       Serial.println("Adjust Left");
// #endif
//           r = RemoteCommandAdjustAccelerometerLeft;
//           break;

//         case 0x00DEED: // TODO pick real value
// #if MAD_REMOTE_LOGGING
//       Serial.println("Adjust Right");
// #endif
//           r = RemoteCommandAdjustAccelerometerRight;
//           break;

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

    // digits
        case 0xFD08F7:
          r = RemoteCommandDigit1;
          break;
        case 0xFD8877:
          r = RemoteCommandDigit2;
          break;
        case 0xFD48B7:
          r = RemoteCommandDigit3;
          break;
        case 0xFD28D7:
          r = RemoteCommandDigit4;
          break;
        case 0xFDA857:
          r = RemoteCommandDigit5;
          break;
        case 0xFD6897:
          r = RemoteCommandDigit6;
          break;
        case 0xFD18E7:
          r = RemoteCommandDigit7;
          break;
        case 0xFD9867:
          r = RemoteCommandDigit8;
          break;
        case 0xFD58A7:
          r = RemoteCommandDigit9;
          break;

      // repeat
        case 0xFFFFFFFF:  // repeat last
          r = RemoteCommandNone;  // ignore duplicate command since we received first
// #if MAD_REMOTE_LOGGING
//       Serial.println("Remote Repeat Last");
// #endif
          break;
      }

   }
    My_Receiver.resume(); 		//Restart the receiver
  }

#ifdef MAD_REMOTE_LOGGING
  if ( r >= RemoteCommandDigit1 && r <= RemoteCommandDigit9 ) {
    unsigned int digit = r - RemoteCommandDigit1 + 1;
    Serial.print("Remote Digit #"); Serial.print(digit); Serial.println("");
  }
#endif

  if ( r != RemoteCommandNone ) {
    newCommand = true;
    last_commands[1] = last_commands[0];
    last_commands[0] = r;
  }

  return r;
}

void RemoteControl::clearCommand() {
  newCommand = false;
}
*/
