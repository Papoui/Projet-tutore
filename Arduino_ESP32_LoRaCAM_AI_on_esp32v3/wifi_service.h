#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFi.h>

struct WifiData{
    String ssid;
    String password;
    bool ap;
};

extern WifiData WifiConfig;
extern const WifiData defaultConfig;
extern bool reloadWifi;
extern unsigned long reloadTime;

void initWifiConfig();
void saveWifiConfig();
void loadWifiConfig();
void resetWifiConfig();
void printWifiConfig();
void initWifiConnection();

#endif