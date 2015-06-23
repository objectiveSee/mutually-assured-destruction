#ifndef Settings_h

#define Settings_h

// ID of the settings block
#define CONFIG_VERSION "ls1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Example settings structure
struct StoreStruct {

  // The variables of your settings
  float accelerometer_angle_positive;
  float accelerometer_angle_negative;

  // char c;
  // long d;
  // float e[6];

  // This is for mere detection if they are your settings
  char version_of_program[4]; // it is the last variable of the struct
  // so when settings are saved, they will only be validated if
  // they are stored completely.
};

extern bool my_settings_loaded;
extern StoreStruct my_settings;

void loadConfig();
void saveConfig();


#endif
// end Settings_h
