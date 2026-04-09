#include "lora_config_service.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "AsyncJson.h"

AsyncWebServer server(8080);

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

void handleIndex(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html", "text/html");
}

void handleNotFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
        return;
    }
    String path = request->url();
    if (LittleFS.exists(path))
    {
        request->send(LittleFS, path);
    }
    else
    {
        request->send(LittleFS, "/index.html", "text/html");
    }
}

void getConfig(AsyncWebServerRequest *request)
{
    JsonDocument doc;
    parseConfigJson(doc, loraConfig);
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void getDefaultConfig(AsyncWebServerRequest *request)
{
    JsonDocument doc;
    parseConfigJson(doc, DEFAULT_LORA_CONFIG);
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void postConfigLora(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    loraConfig.lora.bw = doc["bw"].as<String>();
    loraConfig.lora.sf = doc["sf"].as<String>();
    loraConfig.lora.frequency = doc["frequency"].as<String>();
    loraConfig.lora.devAddr = doc["devAddr"].as<String>();
    loraConfig.lora.appSKey = doc["appSKey"].as<String>();
    loraConfig.lora.nwkSKey = doc["nwkSKey"].as<String>();

    saveConfig();
    request->send(200);
}

void postConfigLoracam(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject doc = json.as<JsonObject>();
    loraConfig.loracam.quality = doc["quality"].as<int>();
    loraConfig.loracam.mss = doc["mss"].as<int>();

    saveConfig();
    request->send(200);
}

void postResetConfig(AsyncWebServerRequest *request)
{
    resetConfig();
    request->send(200);
}

void startWebServer()
{
    initConfig();
    printConfig();

    AsyncCallbackJsonWebHandler* loraHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/lora", postConfigLora);
    AsyncCallbackJsonWebHandler* loracamHandler = new AsyncCallbackJsonWebHandler("/api/lora-config/loracam", postConfigLoracam);
    
    server.on("/", HTTP_GET, handleIndex);
    server.on("/api/lora-config/default", HTTP_GET, getDefaultConfig);
    server.on("/api/lora-config", HTTP_GET, getConfig);
    server.addHandler(loraHandler);
    server.addHandler(loracamHandler);
    server.on("/api/lora-config/reset", HTTP_POST, postResetConfig);
    server.onNotFound(handleNotFound);
    
    server.begin();
}