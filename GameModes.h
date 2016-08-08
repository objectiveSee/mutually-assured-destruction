#ifndef GameModes_h
#define GameModes_h

#include <Arduino.h>

typedef enum  {
  GameModeNone = 0,
  GameModeLighting
} GameMode;

extern GameMode currentGameMode;

bool currentGameModeAllowsImmediatePoof();
bool currentGameModeRequiresSerialAcknowledgementForPoof();

byte serialCommandToRequestPoofAcknowledgement(byte which);
byte serialCommandToReportModeChange(GameMode someMode);

/**
 * Serial API commands
 */
 
// Report poofing (Outgoing)
#define SERIAL_API_COMMAND_REPORT_POOF_LEFT     'L'
#define SERIAL_API_COMMAND_REPORT_POOF_RIGHT    'R'
#define SERIAL_API_COMMAND_REPORT_POOF_BOTH		'B'

 #define SERIAL_API_COMMAND_REQUEST_POOF_LEFT	'A'
#define SERIAL_API_COMMAND_REQUEST_POOF_RIGHT   'S'
#define SERIAL_API_COMMAND_REQUEST_POOF_BOTH    'D'

// Trigger poofing (Incoming)
#define SERIAL_API_COMMAND_TRIGGER_POOF_LEFT    'Q'
#define SERIAL_API_COMMAND_TRIGGER_POOF_RIGHT   'W'
#define SERIAL_API_COMMAND_TRIGGER_POOF_BOTH    'E'

// Switch Modes (Outgoing)
#define SERIAL_API_COMMAND_SWITCH_MODE_NORMAL   'N'
#define SERIAL_API_COMMAND_SWITCH_MODE_LIGHTING 'M'


/**
 * Wireless API Commands
 */
#define WIRELESS_API_COMMAND_TRIGGER_POOF_LEFT      0x07
#define WIRELESS_API_COMMAND_TRIGGER_POOF_RIGHT     0x08
#define WIRELESS_API_COMMAND_TRIGGER_POOF_BOTH      0x09
#define WIRELESS_API_COMMAND_SAVE_ANGLE             0x0A

#define WIRELESS_API_COMMAND_MODE_CHANGE_NORMAL     0x04
#define WIRELESS_API_COMMAND_MODE_CHANGE_LIGHTING   0x05


#endif





