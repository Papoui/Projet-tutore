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

// --- ROUTES GLOBAL ---
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

// --- ROUTES POST ---
void postWifiConfig(AsyncWebServerRequest *request)
{
    if (request->hasArg("ssid"))
        strlcpy(myConfig.ssid, request->arg("ssid").c_str(), sizeof(myConfig.ssid));
    if (request->hasArg("password"))
        strlcpy(myConfig.password, request->arg("password").c_str(), sizeof(myConfig.password));

    myConfig.accessPoint = request->hasArg("accessPoint");

    saveConfig();
    request->send(200, "text/plain", "WiFi Config Sauvegardée");
}

void postLoraConfig(AsyncWebServerRequest *request)
{
    if (request->hasArg("bw"))
        myConfig.bw = request->arg("bw").toInt();
    if (request->hasArg("sf"))
        myConfig.sf = request->arg("sf").toInt();

    if (request->hasArg("frequency"))
        myConfig.frequency = (request->arg("frequency").toFloat());

    if (request->hasArg("devAddr"))
        strlcpy(myConfig.devAddr, request->arg("devAddr").c_str(), sizeof(myConfig.devAddr));
    if (request->hasArg("appSKey"))
        strlcpy(myConfig.appSKey, request->arg("appSKey").c_str(), sizeof(myConfig.appSKey));
    if (request->hasArg("nwkSKey"))
        strlcpy(myConfig.nwkSKey, request->arg("nwkSKey").c_str(), sizeof(myConfig.nwkSKey));

    saveConfig();
    request->send(200, "text/plain", "LoRa Config Sauvegardée");
}

void postLoracamConfig(AsyncWebServerRequest *request)
{
    if (request->hasArg("quality"))
        myConfig.quality = request->arg("quality").toInt();
    if (request->hasArg("mss"))
        myConfig.mss = request->arg("mss").toInt();

    saveConfig();
    request->send(200, "text/plain", "LoRaCam Config Sauvegardée");
}

// --- SERVEUR DE FICHIERS ---
void handleIndex(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html", "text/html");
}

void handleNotFound(AsyncWebServerRequest *request)
{
    String path = request->url();
    if (LittleFS.exists(path))
    {
        request->send(LittleFS, path);
    }
    else
    {
        // On renvoie vers l'index
        request->send(LittleFS, "/index.html", "text/html");
        Serial.printf("404: %s\n", path.c_str());
    }
}

void startWebServer()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("Erreur LittleFS");
        return;
    }

    server.on("/", HTTP_GET, handleIndex);

    // Routes GET
    server.on("/api/config", HTTP_GET, getConfig);
    server.on("/api/config/default", HTTP_GET, getDefaultConfig);

    // Routes POST
    server.on("/api/config/wifi", HTTP_POST, postWifiConfig);
    server.on("/api/config/wifi", HTTP_POST, postLoraConfig);
    server.on("/api/config/wifi", HTTP_POST, postLoracamConfig);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Serveur HTTP (port 8080) démarré");
}