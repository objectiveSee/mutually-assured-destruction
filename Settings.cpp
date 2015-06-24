// FROM: http://playground.arduino.cc/Code/EEPROMLoadAndSaveSettings

/* LoadAndSaveSettings
 * footswitch 2012-03-05, original code by Joghurt (2010)
 * Demonstrates how to load and save settings to the EEPROM
 * Tested on Arduino Uno R2 with Arduino 0023
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>
#include <Arduino.h>
#include "Settings.h"

#define MAD_SETTINGS_LOGGING 1

StoreStruct my_settings = {
  // The default values
  0.2f,
  -0.25f,
  // 220, 1884,
  // 'c',
  // 10000,
  // {4.5, 5.5, 7, 8.5, 10, 12},
  CONFIG_VERSION
};

bool my_settings_loaded = false;

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (//EEPROM.read(CONFIG_START + sizeof(settings) - 1) == settings.version_of_program[3] // this is '\0'
      EEPROM.read(CONFIG_START + sizeof(my_settings) - 2) == my_settings.version_of_program[2] &&
      EEPROM.read(CONFIG_START + sizeof(my_settings) - 3) == my_settings.version_of_program[1] &&
      EEPROM.read(CONFIG_START + sizeof(my_settings) - 4) == my_settings.version_of_program[0])
  { // reads settings from EEPROM
    for (unsigned int t=0; t<sizeof(my_settings); t++) {
      *((char*)&my_settings + t) = EEPROM.read(CONFIG_START + t);
    }

    #if MAD_SETTINGS_LOGGING
    // error writing to EEPROM
    Serial.print("[SETTINGS] Loaded!  Float Value="); Serial.print(my_settings.accelerometer_angle_positive); Serial.println("!");
    #endif

    my_settings_loaded = true;

  } else {
    // settings aren't valid! will overwrite with default settings
    saveConfig();
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(my_settings); t++)
  {
    // writes to EEPROM
    EEPROM.write(CONFIG_START + t, *((char*)&my_settings + t));
    // and verifies the data
    if (EEPROM.read(CONFIG_START + t) != *((char*)&my_settings + t))
    {
      #if MAD_SETTINGS_LOGGING
      // error writing to EEPROM
      Serial.println("[SETTINGS] Write Error");
      #endif
    } else {

      #if MAD_SETTINGS_LOGGING
      // error writing to EEPROM
      Serial.println("[SETTINGS] Write Success!");
      #endif
    }
  }
}

// void setup() {
//   loadConfig();
// }

// void loop() {
//   // [...]
//   int i = settings.c - 'a';
//   // [...]

//   // [...]
//   settings.c = 'a';
//   if (some_condition)
//     saveConfig();
//   // [...]
// }
// [Get Code]
