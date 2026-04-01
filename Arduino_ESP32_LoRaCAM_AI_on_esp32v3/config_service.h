#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <Arduino.h>
#include <EEPROM.h>

struct Config
{
    // Configuration Générale
    bool loadOnBoot;

    // WifiConfig
    char ssid[32];
    char password[64];
    bool accessPoint;

    // LoRaConfig
    int bw;
    int sf;
    int frequency;
    char devAddr[9];  // 8 caractères hexa + '\0'
    char appSKey[33]; // 32 caractères hexa + '\0'
    char nwkSKey[33]; // 32 caractères hexa + '\0'

    // LoRaCamConfig
    int quality;
    int mss;
};

const Config DEFAULT_CONFIG = {
    false,                              // loadOnBoot
    "SFR_6B58",                         // ssid
    "38xbgu4tiv76tyah6rgk",             // password
    false,                              // accessPoint
    125,                                // bw
    12,                                 // sf
    868100000,                          // frequency
    "26012DAA",                         // devAddr
    "23158D3BBC31E6AF670D195B5AED5525", // appSKey
    "23158D3BBC31E6AF670D195B5AED5525", // nwkSKey
    20,                                 // quality
    64                                  // mss
};

extern Config myConfig;

#define EEPROM_SIZE sizeof(Config)

void initConfig();
void saveConfig();
void loadFromMemory();
void resetMemory();
void printConfig();

#endif