#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(8080);

void startWebServer()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("Erreur LittleFS");
        return;
    }

    server.on("/", HTTP_GET,
              [](AsyncWebServerRequest *request) { request->send(LittleFS, "/index.html", "text/html"); });

    server.onNotFound([](AsyncWebServerRequest *request) {
        String path = request->url();

        if (LittleFS.exists(path))
        {
            request->send(LittleFS, path);
        }
        else
        {
            request->send(404, "text/plain", "Erreur 404");
            Serial.printf("404: %s\n", path.c_str());
        }
    });

    server.begin();
    Serial.println("Serveur HTTP (port 8080) démarré");
}