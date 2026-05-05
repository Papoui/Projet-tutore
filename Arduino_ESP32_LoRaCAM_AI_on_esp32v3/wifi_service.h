#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>



void initWifiConfig();
void saveWifiConfig()
void loadWifiConfig();
void resetWifiConfig();
void printWifiConfig();

#endif