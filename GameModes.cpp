/**
 * Game mode logic.
 */

#include "GameModes.h"

/**
* Game Mode Stuff
*/
GameMode currentGameMode = GameModeNone;

bool currentGameModeAllowsImmediatePoof() {
  switch (currentGameMode) {
    case GameModeNone:
      return true;
      break;
    default:
      break;
  }
  return false;
}

bool currentGameModeRequiresSerialAcknowledgementForPoof() {
  switch (currentGameMode) {
    
    case GameModeLighting:
      return true;
      break;

    default:
      break;
  }
  return false;
}

byte serialCommandToReportModeChange(GameMode someMode) {
  switch (someMode) {
    case GameModeLighting:
      return SERIAL_API_COMMAND_SWITCH_MODE_LIGHTING;
      break;

    case GameModeNone:
      return SERIAL_API_COMMAND_SWITCH_MODE_NORMAL;
      break;

    default:
      break;
  }
  return 0;
}

// TODO: sync commands with wolf
byte serialCommandToRequestPoofAcknowledgement(byte which) {
  byte cmd = which & 0x03; // mask off any extra bits
  switch(cmd) {
    case 0x01:
      return SERIAL_API_COMMAND_REQUEST_POOF_LEFT;
      break;
    case 0x02:
      return SERIAL_API_COMMAND_REQUEST_POOF_RIGHT;
      break;
    case 0x03:
      return SERIAL_API_COMMAND_REQUEST_POOF_BOTH;
      break;
    default:
      break;
  }
  return 0; // TODO idk what to do here
}




