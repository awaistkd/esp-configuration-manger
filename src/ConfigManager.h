#ifndef ConfigManager_h
#define ConfigManager_h

#ifdef ESP8266

#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define ESP_getChipId()   (ESP.getChipId())


#else

#include <WiFi.h>
#include <esp_wifi.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#endif

#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <memory>
#include <string>

#define WIFI_CONNECT_TIMEOUT 15


class ConfigManager
{
private:
    const char * hostname;
    StaticJsonDocument<1024> configDoc;

public:
    ConfigManager(const char * hostname);
    void connectOrConfigure();
    bool connectWiFi(std::string ssid, std::string password);
    IPAddress startConfigAP(IPAddress localIp, IPAddress gateway, IPAddress subnet);
    bool loadConfig();
    void startConfigServer();
    ~ConfigManager();
};




#endif