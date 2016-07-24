#ifndef Settings_h
#define Settings_h

struct StoreStruct {
  char version_byte0,version_byte1,version_byte2,version_byte3; // TODO: convert to char[4] or char*
  float accelerometer_angle_positive;
  float accelerometer_angle_negative;
};

extern StoreStruct my_settings;

void loadConfig();
void saveConfig();


#endif  // end Settings_h
