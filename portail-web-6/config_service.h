#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <EEPROM.h>

struct Config
{
    char ssid[32];
    char password[32];
    bool loadOnBoot;
};

const Config DEFAULT_CONFIG = {"SFR_6B58", "38xbgu4tiv76tyah6rgk", false};

extern Config myConfig;

#define EEPROM_SIZE sizeof(Config)

void initConfig();
void saveConfig();
void loadFromMemory();
void resetMemory();

#endif