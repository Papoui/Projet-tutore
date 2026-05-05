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
    WifiConfig.password = doc["password"].as<String>();
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
    doc["password"] = WifiConfig.password;

    // https://arduinojson.org/v7/api/json/serializejson/#write-to-a-file
    serializeJson(doc, file);

    file.close();
    initWifiConnection();
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

void initWifiConnection(){
    initWifiConfig();
    //First, try to connect to an existing network
    WiFi.begin(WifiConfig.ssid.c_str(), WifiConfig.password.c_str());
    Serial.println("Trying to connect to an existing network...");
    Serial.printf("\nRegistered network: %s", WifiConfig.ssid.c_str());
    Serial.printf("\nRegistered password: %s", WifiConfig.password.c_str());
    Serial.print("\nConnecting");
    for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
    {
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED)
    {

        Serial.println("\nFailed to connect. \nSwitching to access point mode...\n");
        WiFi.disconnect(true);

        String localSsid = "ESP32S3";
        String localPassword = "ESP32S3APM";
        WiFi.mode(WIFI_AP);
        WiFi.softAP(localSsid, localPassword);
        Serial.printf("Acces point : %s\n", localSsid);
        Serial.printf("Password : %s\n\n", localPassword);

        Serial.printf("Go to 'http://%s:8080' to connect.\n", WiFi.softAPIP().toString());
    }
    else
    {
        Serial.printf("Go to 'http://%s:8080' to connect.\n", WiFi.localIP().toString());
        
    }
    WiFi.setSleep(false);
}