#include "wifi_service.h"

int initWifi(char* ssid, char* password) {
  WiFi.begin(ssid, password);
  Serial.printf("Tentative de connexion au réseau %s...\n", ssid);
  for (int i = 0;i<5 && WiFi.status()!=WL_CONNECTED;i++) {
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    
    Serial.printf("Impossible de se connecter au réseau en tant que client, passage en mode AP.\n", ssid);
    Serial.printf(ssid);
    WiFi.disconnect(true);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.printf("Point d'accès : %s\n", ssid);

    return 1;
  } 
  else {
    return 2;
  }
  WiFi.setSleep(false);
}