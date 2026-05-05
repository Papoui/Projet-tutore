#include "lora_config_service.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

// ---------------------------------- Docs ----------------------------------

// https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
// https://arduinojson.org

// ---------------------------------- Constantes et variables ----------------------------------

LoraConfig loraConfig;
const char *configFilePath = "/config.json";

const LoraConfig DEFAULT_LORA_CONFIG = {
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

// ---------------------------------- Fonctions ----------------------------------

void initConfig()
{
    if (!LittleFS.begin())
    {
        return;
    }
    loadConfig();
}

void loadConfig()
{
    loraConfig = DEFAULT_LORA_CONFIG;

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
    // https://arduinojson.org/v7/api/json/deserializejson/
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error)
    {
        return;
    }
    
    loraConfig.lora.bw = doc["lora"]["bw"].as<String>();
    loraConfig.lora.sf = doc["lora"]["sf"].as<String>();
    loraConfig.lora.frequency = doc["lora"]["frequency"].as<String>();
    loraConfig.lora.devAddr = doc["lora"]["devAddr"].as<String>();
    loraConfig.lora.appSKey = doc["lora"]["appSKey"].as<String>();
    loraConfig.lora.nwkSKey = doc["lora"]["nwkSKey"].as<String>();

    loraConfig.loracam.quality = doc["loracam"]["quality"].as<int>();
    loraConfig.loracam.mss = doc["loracam"]["mss"].as<int>();
}

void saveConfig()
{
    File file = LittleFS.open(configFilePath, "w");
    if (!file) 
    {
        Serial.printf("LittleFS : %s open failed", configFilePath);
        return;
    }

    JsonDocument doc;
    doc["lora"]["bw"] = loraConfig.lora.bw;
    doc["lora"]["sf"] = loraConfig.lora.sf;
    doc["lora"]["frequency"] = loraConfig.lora.frequency;
    doc["lora"]["devAddr"] = loraConfig.lora.devAddr;
    doc["lora"]["appSKey"] = loraConfig.lora.appSKey;
    doc["lora"]["nwkSKey"] = loraConfig.lora.nwkSKey;
    doc["loracam"]["quality"] = loraConfig.loracam.quality;
    doc["loracam"]["mss"] = loraConfig.loracam.mss;

    // https://arduinojson.org/v7/api/json/serializejson/#write-to-a-file
    serializeJson(doc, file);

    file.close();
}

void resetConfig()
{
    if (LittleFS.exists(configFilePath))
    {
        LittleFS.remove(configFilePath);
    }
    loraConfig = DEFAULT_LORA_CONFIG;
}

void printConfig()
{
    Serial.println("--- Configuration lora actuelle ---");
    Serial.print("BW : ");
    Serial.println(loraConfig.lora.bw);
    Serial.print("SF : ");
    Serial.println(loraConfig.lora.sf);
    Serial.print("Frequency : ");
    Serial.println(loraConfig.lora.frequency);
    Serial.print("DevAddr : ");
    Serial.println(loraConfig.lora.devAddr);
    Serial.print("AppSKey : ");
    Serial.println(loraConfig.lora.appSKey);
    Serial.print("NwkSKey : ");
    Serial.println(loraConfig.lora.nwkSKey);
    Serial.print("Quality : ");
    Serial.println(loraConfig.loracam.quality);
    Serial.print("MSS : ");
    Serial.println(loraConfig.loracam.mss);
    Serial.println("---------------------------------");
}