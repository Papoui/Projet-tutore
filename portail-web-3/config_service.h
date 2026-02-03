#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <EEPROM.h>

struct Config {
  char text[32];
  int integer;
  float number;
  bool boolean;
  bool loadOnBoot;
};

const Config DEFAULT_CONFIG = {"Defaut", 0, 1.0, false, false};

extern Config myConfig;

#define EEPROM_SIZE sizeof(Config)

void initEEPROM();
void saveToEEPROM();
void loadFromEEPROM();

#endif