#ifndef LORA_CONFIG_SERVICE_H
#define LORA_CONFIG_SERVICE_H

#include <Arduino.h>

struct LoraConfig
{
    String bw;
    String sf;
    String frequency;
    String devAddr;
    String appSKey;
    String nwkSKey;
};

struct LoraCamConfig
{
    int quality;
    int mss;
};

struct Config
{
    LoraConfig lora;
    LoraCamConfig loracam;
};

extern Config myConfig;
extern const Config DEFAULT_LORA_CONFIG;

void initConfig();
void saveConfig();
void loadFromMemory();
void printConfig();
void resetMemory();

#endif