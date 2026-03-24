#include "config_service.h"

Config myConfig;

void initConfig()
{
    EEPROM.begin(EEPROM_SIZE);
    loadFromMemory();
    if (myConfig.loadOnBoot)
    {
        Serial.println("Config chargee au demarrage.");
    }
    else
    {
        Serial.println("Chargement ignore (Option desactivee).");
        myConfig = DEFAULT_CONFIG;
    }
}

void saveConfig()
{
    Serial.println("--- Sauvegarde en cours ---");
    EEPROM.put(0, myConfig);
    if (EEPROM.commit())
    {
        Serial.println("Succès : Données écrites en EEPROM.");
    }
    else
    {
        Serial.println("Erreur : Échec EEPROM !");
    }
}

void loadFromMemory()
{
    Serial.println("--- Chargement depuis l'EEPROM ---");

    EEPROM.get(0, myConfig);

    Serial.print("ssid : ");
    Serial.println(myConfig.ssid);
    Serial.print("mot de passe : ");
    Serial.println(myConfig.password);
    Serial.print("loadOnBoot : ");
    Serial.println(myConfig.loadOnBoot ? "True" : "False");
    Serial.println("Chargement terminé.");
}

void resetMemory()
{
    Serial.println("--- Lancement de la réinitialisation de l'EEPROM ---");
    EEPROM.begin(EEPROM_SIZE);
    myConfig = DEFAULT_CONFIG;
    saveConfig();
    Serial.println("--- EEPROM réinitialisée avec succès ---");
}