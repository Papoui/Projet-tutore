#include "config_service.h"

Config myConfig;

void initEEPROM() {
  EEPROM.begin(EEPROM_SIZE);

  loadFromEEPROM();
  
  if (myConfig.loadOnBoot) {
    Serial.println("Config chargee au demarrage.");
  } else {
    Serial.println("Chargement ignore (Option desactivee).");
    myConfig = DEFAULT_CONFIG;
  }
}

void saveToEEPROM() {
  Serial.println("--- Sauvegarde en cours ---");
  EEPROM.put(0, myConfig);
  if (EEPROM.commit()) {
    Serial.println("Succès : Données écrites en EEPROM.");
    Serial.print("Taille écrite : ");
    Serial.print(EEPROM_SIZE);
    Serial.println(" bytes.");
  } else {
    Serial.println("Erreur : Échec du commit EEPROM !");
  }
}

void loadFromEEPROM() {
  Serial.println("--- Chargement depuis l'EEPROM ---");
 
  EEPROM.get(0, myConfig);
 
  Serial.print("Texte : ");  Serial.println(myConfig.text);
  Serial.print("Entier : "); Serial.println(myConfig.integer);
  Serial.print("Float : ");  Serial.println(myConfig.number);
  Serial.print("Bool : ");   Serial.println(myConfig.boolean ? "True" : "False");
  Serial.print("loadOnBoot : ");   Serial.println(myConfig.loadOnBoot ? "True" : "False");
  Serial.println("Chargement terminé.");
}