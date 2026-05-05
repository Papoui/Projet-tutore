#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

struct WifiData{
    String ssid;
    String password;
};

void initWifiConfig();
void saveWifiConfig();
void loadWifiConfig();
void resetWifiConfig();
void printWifiConfig();

#endif