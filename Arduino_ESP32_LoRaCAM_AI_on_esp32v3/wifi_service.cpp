#include "wifi_service.h"

const char *WifiConfigFilePath = "/wifi_config.json";

bool reloadWifi = false;
unsigned long reloadTime = 0;
WifiData WifiConfig;
const WifiData defaultConfig = {
    "",
    "",
    1
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
    WifiConfig.ap = doc["ap"].as<int>();
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
    doc["ap"] = WifiConfig.ap;

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

void initWifiConnection(){
    //First, try to connect to an existing network
    WiFi.mode(WIFI_AP_STA);
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
        Serial.printf("Acces point : %s\n", localSsid.c_str());
        Serial.printf("Password : %s\n\n", localPassword.c_str());
        Serial.printf("\nSuccess! Go to 'http://%s:8080' to connect.\n", WiFi.softAPIP().toString().c_str());
    }
    else
    {
        Serial.printf("\nSucces! Go to 'http://%s:8080' to connect.\n", WiFi.localIP().toString().c_str());
        saveWifiConfig();
        if(WifiConfig.ap == false){
            WiFi.mode(WIFI_STA);
        }else{
            Serial.printf("\n(Acces point :Go to 'http://%s:8080' to connect.)\n", WiFi.softAPIP().toString().c_str());
        }
        
    }
    WiFi.setSleep(false);
}