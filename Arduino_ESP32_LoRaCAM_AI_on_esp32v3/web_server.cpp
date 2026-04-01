#include "config_service.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(8080);

void parseConfigJson(JsonDocument &doc, const Config &c)
{
    doc["loadOnBoot"] = c.loadOnBoot;
    doc["ssid"] = c.ssid;
    doc["password"] = c.password;
    doc["accessPoint"] = c.accessPoint;
    doc["bw"] = c.bw;
    doc["sf"] = c.sf;
    doc["frequency"] = c.frequency;
    doc["devAddr"] = c.devAddr;
    doc["appSKey"] = c.appSKey;
    doc["nwkSKey"] = c.nwkSKey;
    doc["quality"] = c.quality;
    doc["mss"] = c.mss;
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
    if (request->method() == HTTP_OPTIONS) {
        request->send(200);
        return;
    }

    String path = request->url();
    if (LittleFS.exists(path)) {
        request->send(LittleFS, path);
    } else {
        request->send(LittleFS, "/index.html", "text/html");
        Serial.printf("404: %s (Methode: %s)\n", path.c_str(), request->methodToString());
    }
}

void handleEmptyRequest(AsyncWebServerRequest *request)
{
}

void handleWifiBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0 && len == total) {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len)) {
            if (doc.containsKey("ssid")) strlcpy(myConfig.ssid, doc["ssid"], sizeof(myConfig.ssid));
            if (doc.containsKey("password")) strlcpy(myConfig.password, doc["password"], sizeof(myConfig.password));
            myConfig.accessPoint = doc.containsKey("accessPoint") && (doc["accessPoint"] == "on" || doc["accessPoint"] == true);
            
            saveConfig();
            printConfig();
            request->send(200, "text/plain", "WiFi Config Sauvegardee");
        } else {
            request->send(400, "text/plain", "JSON Invalide");
        }
    }
}

void handleLoraBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0 && len == total) {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len)) {
            if (doc.containsKey("bw")) myConfig.bw = doc["bw"].as<int>();
            if (doc.containsKey("sf")) myConfig.sf = doc["sf"].as<int>();
            if (doc.containsKey("frequency")) myConfig.frequency = doc["frequency"].as<int>();
            if (doc.containsKey("devAddr")) strlcpy(myConfig.devAddr, doc["devAddr"], sizeof(myConfig.devAddr));
            if (doc.containsKey("appSKey")) strlcpy(myConfig.appSKey, doc["appSKey"], sizeof(myConfig.appSKey));
            if (doc.containsKey("nwkSKey")) strlcpy(myConfig.nwkSKey, doc["nwkSKey"], sizeof(myConfig.nwkSKey));
            
            saveConfig();
            request->send(200, "text/plain", "LoRa Config Sauvegardee");
        } else {
            request->send(400, "text/plain", "JSON Invalide");
        }
    }
}

void handleLoracamBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (index == 0 && len == total) {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len)) {
            if (doc.containsKey("quality")) myConfig.quality = doc["quality"].as<int>();
            if (doc.containsKey("mss")) myConfig.mss = doc["mss"].as<int>();
            
            saveConfig();
            request->send(200, "text/plain", "LoRaCam Config Sauvegardee");
        } else {
            request->send(400, "text/plain", "JSON Invalide");
        }
    }
}

void startWebServer()
{
    if (!LittleFS.begin(true)) {
        Serial.println("Erreur LittleFS");
        return;
    }

    server.on("/", HTTP_GET, handleIndex);
    server.on("/api/config", HTTP_GET, getConfig);
    server.on("/api/config/default", HTTP_GET, getDefaultConfig);

    server.on("/api/config/wifi", HTTP_POST, handleEmptyRequest, NULL, handleWifiBody);
    server.on("/api/config/lora", HTTP_POST, handleEmptyRequest, NULL, handleLoraBody);
    server.on("/api/config/loracam", HTTP_POST, handleEmptyRequest, NULL, handleLoracamBody);

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Serveur HTTP (port 8080) demarre");
}