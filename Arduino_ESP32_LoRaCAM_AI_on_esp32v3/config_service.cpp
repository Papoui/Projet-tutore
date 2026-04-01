#include "config_service.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

Config myConfig;
const char *configFilePath = "/lora_config.json";

const Config DEFAULT_CONFIG = {
    {
        "125", // bw
        "12", // sf
        "868.1", // fréquence 
        "26012DAA", // device address
        "23158D3BBC31E6AF670D195B5AED5525", // AppSKey 
        "23158D3BBC31E6AF670D195B5AED5525" // NwkSKey
    },
    {
        20, // Quality Factor
        64 // MSS
    }
};

void initConfig()
{
    if (!LittleFS.begin(true))
        return;
    loadFromMemory();
}

void loadFromMemory()
{
    myConfig = DEFAULT_CONFIG;

    if (!LittleFS.exists(configFilePath))
    {
        return;
    }
    
    File file = LittleFS.open(configFilePath, "r");
    if (!file)
    {
        return;
    }
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        return;
    }
    
    myConfig.lora.bw = doc["lora"]["bw"].as<String>();
    myConfig.lora.sf = doc["lora"]["sf"].as<String>();
    myConfig.lora.frequency = doc["lora"]["frequency"].as<String>();
    myConfig.lora.devAddr = doc["lora"]["devAddr"].as<String>();
    myConfig.lora.appSKey = doc["lora"]["appSKey"].as<String>();
    myConfig.lora.nwkSKey = doc["lora"]["nwkSKey"].as<String>();

    myConfig.loracam.quality = doc["loracam"]["quality"].as<int>();
    myConfig.loracam.mss = doc["loracam"]["mss"].as<int>();
}

void saveConfig()
{
    File file = LittleFS.open(configFilePath, "w");
    if (!file) 
    {
        return;
    }

    JsonDocument doc;
    doc["lora"]["bw"] = myConfig.lora.bw;
    doc["lora"]["sf"] = myConfig.lora.sf;
    doc["lora"]["frequency"] = myConfig.lora.frequency;
    doc["lora"]["devAddr"] = myConfig.lora.devAddr;
    doc["lora"]["appSKey"] = myConfig.lora.appSKey;
    doc["lora"]["nwkSKey"] = myConfig.lora.nwkSKey;
    doc["loracam"]["quality"] = myConfig.loracam.quality;
    doc["loracam"]["mss"] = myConfig.loracam.mss;

    serializeJson(doc, file);
    file.close();
}

void printConfig()
{
    Serial.print("BW : ");
    Serial.println(myConfig.lora.bw);
    Serial.print("SF : ");
    Serial.println(myConfig.lora.sf);
    Serial.print("Frequency : ");
    Serial.println(myConfig.lora.frequency);
    Serial.print("DevAddr : ");
    Serial.println(myConfig.lora.devAddr);
    Serial.print("AppSKey : ");
    Serial.println(myConfig.lora.appSKey);
    Serial.print("NwkSKey : ");
    Serial.println(myConfig.lora.nwkSKey);
    Serial.print("Quality : ");
    Serial.println(myConfig.loracam.quality);
    Serial.print("MSS : ");
    Serial.println(myConfig.loracam.mss);
}

void resetMemory()
{
    if (LittleFS.exists(configFilePath))
    {
        LittleFS.remove(configFilePath);
    }
    myConfig = DEFAULT_CONFIG;
}