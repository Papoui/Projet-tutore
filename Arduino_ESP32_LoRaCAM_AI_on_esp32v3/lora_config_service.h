#ifndef LORA_CONFIG_SERVICE_H
#define LORA_CONFIG_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct LoraData
{
    String bw;
    String sf;
    String frequency;
    String devAddr;
    String appSKey;
    String nwkSKey;
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
unsigned char stringToUnsignedChar();

#endif