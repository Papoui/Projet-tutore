#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

#define WIFI_SSID "SFR_6B58"
#define WIFI_PASSWORD "38xbgu4tiv76tyah6rgk"

struct Config {
  char text[32];
  int integer;
  float number;
  bool boolean;
  bool loadOnBoot;
};

Config myConfig;
WebServer server(80);

#define EEPROM_SIZE sizeof(Config)

void saveToEEPROM() {
  EEPROM.put(0, myConfig);
  EEPROM.commit();
}

void loadFromEEPROM() {
  EEPROM.get(0, myConfig);
}

void handleRoot() {
  String html = "<html><body><h1>Config XIAO</h1><form action='/save' method='POST'>";
  html += "Texte: <input type='text' name='text' value='" + String(myConfig.text) + "'><br>";
  html += "Entier: <input type='number' name='integer' value='" + String(myConfig.integer) + "'><br>";
  html += "Flotant: <input type='number' step='0.1' name='float' value='" + String(myConfig.number) + "'><br>";
  html += "Booléen: <input type='checkbox' name='boolean' " + String(myConfig.boolean ? "checked" : "") + "><br>";
  html += "<b>Lire au demarrage:</b> <input type='checkbox' name='boot' " + String(myConfig.loadOnBoot ? "checked" : "") + "><br><br>";
  html += "<input type='submit' value='Sauvegarder'></form>";
  html += "<button onclick=\"location.href='/read'\">Lire la mémoire (Manuel)</button>";
  html += "</body></html>";
  server.send(200, "text/html; charset=utf-8", html);
}

void handleSave() {
  if (server.hasArg("text")) strncpy(myConfig.text, server.arg("text").c_str(), 32);
  myConfig.integer = server.arg("integer").toInt();
  myConfig.number = server.arg("float").toFloat();
  myConfig.boolean = server.hasArg("boolean");
  myConfig.loadOnBoot = server.hasArg("boot");

  saveToEEPROM();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRead() {
  loadFromEEPROM(); 
  server.sendHeader("Location", "/");
  server.send(303); 
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // LOGIQUE DE DÉMARRAGE
  Config temp;
  EEPROM.get(0, temp);
  
  if (temp.loadOnBoot) {
    myConfig = temp;
    Serial.println("✅ Config chargee au demarrage.");
  } else {
    Serial.println("⚠️ Chargement ignore (Option desactivee).");
    // Valeurs par défaut si non chargé
    myConfig = {"Defaut", 0, 1.0, false, false};
  }

  // Connexion WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  // Routes
  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/read", handleRead); 
  server.begin();
}

void loop() {
  server.handleClient();
}