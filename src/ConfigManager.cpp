#include "ConfigManager.h"
#include "ConfigServer.h"
#include <string>
#include <SPIFFS.h>

ConfigManager::ConfigManager(const char * _hostname) {
    hostname = hostname;
}

void ConfigManager::connectOrConfigure() {

  bool configError = false;

    if(SPIFFS.exists("/config.json")) {
        if(loadConfig()) {

            std::string ssid = configDoc["ssid"];
            std::string password = configDoc["password"];

            if(!connectWiFi(ssid, password)) {
                Serial.println("Unable to connect to wifi/ wrong crendetials");
                configError = true;
            }

        } else {
            configError = true;
            Serial.print("Config file do not exists. ");
        }
    }


    if( configError || WiFi.status() != WL_CONNECTED) {
        
        IPAddress ip(192, 168, 1, 1);
        IPAddress gateway(192, 168, 1, 1);
        IPAddress subnet(255, 255, 255, 0);

        startConfigAP(ip, gateway, subnet);
        startConfigServer();

    }

}

bool ConfigManager::connectWiFi( std::string ssid, std::string password ) {

    WiFi.begin(ssid.c_str(), password.c_str());

    int timePassed = 0;

    while( WiFi.status() != WL_CONNECTED ) {
        delay(1000);
        if(timePassed > WIFI_CONNECT_TIMEOUT) { return false; }
        timePassed++;
    }

    return true;
}

IPAddress ConfigManager::startConfigAP(IPAddress localIp, IPAddress gateway, IPAddress subnet) {

    WiFi.mode(WIFI_AP);
    
    WiFi.softAP("ESP-AP", "");

    delay(100);

    if( WiFi.softAPConfig(localIp, gateway, subnet) ) {
        Serial.println("Soft AP Configured");
    } else {
        Serial.println("Error Configuring Soft AP");
    }
    
    IPAddress myIP = WiFi.softAPIP();
    return myIP;
}

void ConfigManager::startConfigServer() {
    ConfigServer server(configDoc);
    server.setup();
    server.start();
}

bool ConfigManager::loadConfig() {

    File configFile = SPIFFS.open("/config.json", "r");

    if (!configFile) {
        return false;
    }

    size_t size = configFile.size();
    if (size > 1024) {
        return false;
    }

    auto error = deserializeJson(configDoc, configFile);

    configFile.close();

    if (error) {
        return false;
    }

}

ConfigManager::~ConfigManager() {
    
}
