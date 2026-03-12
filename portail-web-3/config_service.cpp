#include "config_service.h"

Config myConfig;

void initConfig()
{
    EEPROM.begin(EEPROM_SIZE);

    loadFromEEPROM();

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

void saveToEEPROM()
{
    Serial.println("--- Sauvegarde en cours ---");
    EEPROM.put(0, myConfig);
    if (EEPROM.commit())
    {
        Serial.println("Succès : Données écrites en EEPROM.");
    }
    else
    {
        Serial.println("Erreur : Échec du commit EEPROM !");
    }
}

void loadFromEEPROM()
{
    EEPROM.get(0, myConfig);
}