#ifndef ConfigServer_h
#define ConfigServer_h

#ifdef ESP8266
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif

#include <memory>
#include <ArduinoJson.h>
#include <SPIFFS.h>

class ConfigServer
{
private:
    #ifdef ESP8266
    std::unique_ptr<ESP8266WebServer> server;
    #else
    std::unique_ptr<WebServer> server;
    #endif

    StaticJsonDocument<1024> configDoc;

public:
    ConfigServer(StaticJsonDocument<1024>& _configDoc);
    void setup();
    void start();
    void handleRoot();
    void handleDeviceSetup();
    void handleSaveConfig();
    void handleCurrentConfigs();
    void handleReboot();
    ~ConfigServer();
};

#endif