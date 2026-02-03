#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

void startWebServer() {
  AsyncWebServer server(8080);

  if(!LittleFS.begin(true)) { 
    Serial.println("Erreur LittleFS"); 
    return; 
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/config.html", "text/html");
  });

  server.begin();
}