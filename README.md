# ESP32 configurable interface

This project was developed as part of a Master 1 (M1) tutored project at UPPA (Université de Pau et des Pays de l'Adour).

That fork adds the following functionalities :

### Web Server

- Another web server is running on port 8080, serving configuration pages to tweak network parameters such as WiFi or LoRa.

- All pages were created considering responsiveness so it can be easily displayed on small screens like phones.

### WiFi handling

- The ESP first attempts to connect to an existing network, assuming that it has already been registered on the device. Otherwise, it switches to access point mode.

### File handling

- `lora_config_service.cpp` and `wifi_service.cpp` uses LittleFS to store and read data.

- `web_server.cpp` uses LittleFS to route file to the client.
