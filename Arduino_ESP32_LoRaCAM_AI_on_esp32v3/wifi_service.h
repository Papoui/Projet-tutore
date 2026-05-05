#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

struct WifiData{
    String ssid;
    String password;
};

extern WifiData WifiConfig;
extern const WifiData defaultConfig;

void initWifiConfig();
void saveWifiConfig();
void loadWifiConfig();
void resetWifiConfig();
void printWifiConfig();

#endif