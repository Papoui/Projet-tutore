#ifndef LORA_CONFIG_SERVICE_H
#define LORA_CONFIG_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct LoraData
{
    String bw;
    String sf;
    String frequency;
    unsigned char devAddr[4];
    unsigned char appSKey[16];
    unsigned char nwkSKey[16];
};

struct LoraCamData
{
    int quality;
    int mss;
};

struct LoraConfig
{
    LoraData lora;
    LoraCamData loracam;
};

extern LoraConfig loraConfig;
extern const LoraConfig DEFAULT_LORA_CONFIG;

void initConfig();
void saveConfig();
void loadConfig();
void resetConfig();
void printConfig();

#endif