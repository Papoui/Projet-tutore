#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <EEPROM.h>

struct Config
{
    char ssid[32];
    char password[32];
    bool loadOnBoot;
};

const Config DEFAULT_CONFIG = {"", "", false};

extern Config myConfig;

#define EEPROM_SIZE sizeof(Config)

void initConfig();
void saveToEEPROM();
void loadFromEEPROM();

#endif