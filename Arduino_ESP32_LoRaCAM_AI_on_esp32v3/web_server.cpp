#include "lora_config_service.h"
#include "wifi_service.h"
#include <ArduinoJson.h> 
#include "AsyncJson.h"
#include <ESPAsyncWebServer.h> 
#include <LittleFS.h>

// ---------------------------------- Docs ----------------------------------

// https://arduinojson.org
// https://esp32async.github.io/ESPAsyncWebServer

// ---------------------------------- Utils ----------------------------------

void parseLoraConfigJson(JsonDocument &doc, const LoraConfig &config)
{
    doc["lora"]["bw"] = config.lora.bw;
    doc["lora"]["sf"] = config.lora.sf;
    doc["lora"]["frequency"] = config.lora.frequency;
    doc["lora"]["devAddr"] = config.lora.devAddr;
    doc["lora"]["appSKey"] = config.lora.appSKey;
    doc["lora"]["nwkSKey"] = config.lora.nwkSKey;
    doc["loracam"]["quality"] = config.loracam.quality;
    doc["loracam"]["mss"] = config.loracam.mss;
}

void parseWifiConfigJson(JsonDocument &doc, const WifiData &config)
{
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;
}

// ---------------------------------- Routes des fichiers ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/responses/#respond-with-content-coming-from-a-file

void getIndex(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html");
}

// Quand la route n'est pas définit, on cherche un fichier, s'il existe on le renvoie, sinon on renvoie index.html
void onNotFound(AsyncWebServerRequest *request)
{
    String path = request->url();
    if (LittleFS.exists(path))
    {
        request->send(LittleFS, path);
    }
    else
    {
        request->send(LittleFS, "/index.html");
    }
}

// ---------------------------------- Routes de configuration Lora ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/responses/#arduinojson-basic-response

void getLoraConfig(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseLoraConfigJson(doc, loraConfig);
    serializeJson(doc, *response);
    request->send(response);
}

void getLoraDefaultConfig(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseLoraConfigJson(doc, DEFAULT_LORA_CONFIG);
    serializeJson(doc, *response);
    request->send(response);
}

// https://esp32async.github.io/ESPAsyncWebServer/requests/#json-body-handling-with-arduinojson

void postLoraConfig(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    loraConfig.lora.bw = doc["bw"].as<String>();
    loraConfig.lora.sf = doc["sf"].as<String>();
    loraConfig.lora.frequency = doc["frequency"].as<String>();
    loraConfig.lora.devAddr = doc["devAddr"].as<String>();
    loraConfig.lora.appSKey = doc["appSKey"].as<String>();
    loraConfig.lora.nwkSKey = doc["nwkSKey"].as<String>();

    saveLoraConfig();
    request->send(200);
}

void postLoracamConfig(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    loraConfig.loracam.quality = doc["quality"].as<int>();
    loraConfig.loracam.mss = doc["mss"].as<int>();

    saveLoraConfig();
    request->send(200);
}

void postLoraConfigReset(AsyncWebServerRequest *request)
{
    resetLoraConfig();
    request->send(200);
}

// ---------------------------------- Routes de configuration Wifi ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/responses/#arduinojson-basic-response

void getWifiConfig(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseWifiConfigJson(doc, WifiConfig);
    serializeJson(doc, *response);
    request->send(response);
}

void getWifiDefaultConfig(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseWifiConfigJson(doc, defaultConfig);
    serializeJson(doc, *response);
    request->send(response);
}

// https://esp32async.github.io/ESPAsyncWebServer/requests/#json-body-handling-with-arduinojson

void postWifiConfig(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    WifiConfig.ssid = doc["ssid"].as<String>();
    WifiConfig.password = doc["password"].as<String>();

    saveWifiConfig();
    request->send(200);
}

void postWifiConfigReset(AsyncWebServerRequest *request)
{
    resetWifiConfig();
    request->send(200);
}

// ---------------------------------- Lancement ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/setup/

AsyncWebServer server(8080);

void startWebServer()
{
    initWifiConfig();
    initLoraConfig();
    //printLoraConfig();
    //printWifiConfig();

    AsyncCallbackJsonWebHandler* loraHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/lora", postLoraConfig);
    AsyncCallbackJsonWebHandler* loracamHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/loracam", postLoracamConfig);
    AsyncCallbackJsonWebHandler* WifiHandler = new AsyncCallbackJsonWebHandler("/api/wifi-config", postWifiConfig);
    
    server.on("/", HTTP_GET, getIndex);
    server.on("/api/lora-config/default", HTTP_GET, getLoraDefaultConfig);
    server.on("/api/lora-config", HTTP_GET, getLoraConfig);
    server.on("/api/lora-config/reset", HTTP_POST, postLoraConfigReset);
    server.addHandler(loraHandler);
    server.addHandler(loracamHandler);

    server.on("/api/wifi-config/default", HTTP_GET, getWifiDefaultConfig);
    server.on("/api/wifi-config", HTTP_GET, getWifiConfig);
    server.on("/api/wifi-config/reset", HTTP_POST, postWifiConfigReset);
    server.addHandler(WifiHandler);

    server.onNotFound(onNotFound);
    
    server.begin();
}