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

void parseConfigJson(JsonDocument &doc, const LoraConfig &config)
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

// ---------------------------------- Routes de configuration ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/responses/#arduinojson-basic-response

void getConfig(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseConfigJson(doc, loraConfig);
    serializeJson(doc, *response);
    request->send(response);
}

void getConfigDefault(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;
    parseConfigJson(doc, DEFAULT_LORA_CONFIG);
    serializeJson(doc, *response);
    request->send(response);
}

// https://esp32async.github.io/ESPAsyncWebServer/requests/#json-body-handling-with-arduinojson

void postConfigLora(AsyncWebServerRequest *request, JsonVariant &json)
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

void postConfigLoracam(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    loraConfig.loracam.quality = doc["quality"].as<int>();
    loraConfig.loracam.mss = doc["mss"].as<int>();

    saveLoraConfig();
    request->send(200);
}

void postConfigReset(AsyncWebServerRequest *request)
{
    resetLoraConfig();
    request->send(200);
}

// ---------------------------------- Lancement ----------------------------------

// https://esp32async.github.io/ESPAsyncWebServer/setup/

AsyncWebServer server(8080);

void startWebServer()
{
    initLoraConfig();
    printLoraConfig();

    AsyncCallbackJsonWebHandler* loraHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/lora", postConfigLora);
    AsyncCallbackJsonWebHandler* loracamHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/loracam", postConfigLoracam);
    
    server.on("/", HTTP_GET, getIndex);
    server.on("/api/lora-config/default", HTTP_GET, getConfigDefault);
    server.on("/api/lora-config", HTTP_GET, getConfig);
    server.addHandler(loraHandler);
    server.addHandler(loracamHandler);
    server.on("/api/lora-config/reset", HTTP_POST, postConfigReset);
    server.onNotFound(onNotFound);
    
    server.begin();
}