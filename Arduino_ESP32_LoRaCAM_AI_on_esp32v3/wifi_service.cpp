#include "wifi_service.h"

const char *WifiConfigFilePath = "/wifi_config.json";

WifiData WifiConfig;
const WifiData defaultConfig = {
    "",
    ""
};

void initWifiConfig()
{
    if (!LittleFS.begin())
    {
        return;
    }
    loadWifiConfig();
}

void loadWifiConfig()
{
    WifiConfig = defaultConfig;

    if (!LittleFS.exists(WifiConfigFilePath))
    {
        return;
    }
    
    File file = LittleFS.open(WifiConfigFilePath, "r");
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
    
    WifiConfig.ssid = doc["ssid"].as<String>();
    WifiConfig.ssid = doc["password"].as<String>();
}

void saveWifiConfig()
{
    File file = LittleFS.open(WifiConfigFilePath, "w");
    if (!file) 
    {
        Serial.printf("LittleFS : %s open failed", WifiConfigFilePath);
        return;
    }

    JsonDocument doc;
    doc["ssid"] = WifiConfig.ssid;
    doc["password"] = WifiConfig.ssid;

    // https://arduinojson.org/v7/api/json/serializejson/#write-to-a-file
    serializeJson(doc, file);

    file.close();
}

void resetWifiConfig()
{
    if (LittleFS.exists(WifiConfigFilePath))
    {
        LittleFS.remove(WifiConfigFilePath);
    }
    WifiConfig = defaultConfig;
}

void printWifiConfig()
{
    Serial.println("--- Current Wifi parameters ---");
    Serial.print("Ssid : ");
    Serial.println(WifiConfig.ssid);
    Serial.print("Password : ");
    Serial.println(WifiConfig.password);
    Serial.println("---------------------------------");
}