#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(8080);

void handleIndex(AsyncWebServerRequest *request)
{
    request->send(LittleFS, "/index.html", "text/html");
}

void handleWifiConfig(AsyncWebServerRequest *request)
{
    if (request->hasParam("ssid"))
    {
        String value = request->getParam("ssid")->value();
        Serial.printf("ssid : %s\n", value.c_str());
    }
    if (request->hasParam("password"))
    {
        String value = request->getParam("password")->value();
        Serial.printf("password : %s\n", value.c_str());
    }
    request->send(200);
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
        request->send(404, "text/plain", "Fichier introuvable");
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
    server.on("/api/config/wifi", HTTP_POST, handleWifiConfig);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Serveur HTTP (port 8080) démarré");
}
