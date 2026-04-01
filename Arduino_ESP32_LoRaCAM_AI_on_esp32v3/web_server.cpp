#include "config_service.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(8080);

void parseConfigJson(JsonDocument &doc, const Config &c)
{
    doc["lora"]["bw"] = c.lora.bw;
    doc["lora"]["sf"] = c.lora.sf;
    doc["lora"]["frequency"] = c.lora.frequency;
    doc["lora"]["devAddr"] = c.lora.devAddr;
    doc["lora"]["appSKey"] = c.lora.appSKey;
    doc["lora"]["nwkSKey"] = c.lora.nwkSKey;
    doc["loracam"]["quality"] = c.loracam.quality;
    doc["loracam"]["mss"] = c.loracam.mss;
}

void getConfig(AsyncWebServerRequest *request)
{
    JsonDocument doc;
    parseConfigJson(doc, myConfig);
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void getDefaultConfig(AsyncWebServerRequest *request)
{
    JsonDocument doc;
    parseConfigJson(doc, DEFAULT_CONFIG);
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
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

void handleEmptyRequest(AsyncWebServerRequest *request)
{
}

void handleLoraBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0 && len == total)
    {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len))
        {
            myConfig.lora.bw = doc["bw"].as<String>();
            myConfig.lora.sf = doc["sf"].as<String>();
            myConfig.lora.frequency = doc["frequency"].as<String>();
            myConfig.lora.devAddr = doc["devAddr"].as<String>();
            myConfig.lora.appSKey = doc["appSKey"].as<String>();
            myConfig.lora.nwkSKey = doc["nwkSKey"].as<String>();

            saveConfig();
            printConfig();
            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(400, "text/plain", "Erreur JSON");
        }
    }
}

void handleLoracamBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0 && len == total)
    {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len))
        {
            myConfig.loracam.quality = doc["quality"].as<int>();
            myConfig.loracam.mss = doc["mss"].as<int>();

            saveConfig();
            printConfig();
            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(400, "text/plain", "Erreur JSON");
        }
    }
}

void startWebServer()
{
    initConfig();
    printConfig(); 
    server.on("/", HTTP_GET, handleIndex);
    server.on("/api/config", HTTP_GET, getConfig);
    server.on("/api/config/default", HTTP_GET, getDefaultConfig);
    server.on("/api/config/lora", HTTP_POST, handleEmptyRequest, NULL, handleLoraBody);
    server.on("/api/config/loracam", HTTP_POST, handleEmptyRequest, NULL, handleLoracamBody);
    server.onNotFound(handleNotFound);
    server.begin();
}