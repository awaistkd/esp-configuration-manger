
#include "ConfigServer.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <memory>

ConfigServer::ConfigServer(StaticJsonDocument<1024>& _configDoc)
{

    configDoc = _configDoc;

    #ifdef ESP8266
    server.reset(new ESP8266WebServer(80));
    #else
    server.reset(new WebServer(80));
    #endif

}

void ConfigServer::setup() {
    server->on("/", std::bind(&ConfigServer::handleRoot, this));
    server->on("/deviceSetup", std::bind(&ConfigServer::handleDeviceSetup, this));
    server->on("/saveConfig", std::bind(&ConfigServer::handleSaveConfig, this));
    server->on("/currentConfigs", std::bind(&ConfigServer::handleCurrentConfigs, this));
    server->on("/reboot", std::bind(&ConfigServer::handleReboot, this));
}

void ConfigServer::start() {
    server->begin();
    while(true) {
        server->handleClient();
    }
}

void ConfigServer::handleRoot() {
    const char INDEX[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Device Portal</title><style>html{box-sizing:border-box;font-size:16px}*,*:before,*:after{box-sizing:inherit}body,h1,h2,h3,h4,h5,h6,p,ol,ul{margin:0;padding:0;font-weight:400}ol,ul{list-style:none}body{font-family:Arial,Helvetica,sans-serif;margin-top:10px}img{max-width:100%;height:auto}.container{width:90%;max-width:450px;margin:0 auto}h1{text-align:center}.btn{text-align:center;height:40px;margin-top:10px}.container a{display:inline-block;text-decoration:none;color:#fff;background-color:#000;border-radius:3px;vertical-align:baseline;height:40px;width:100%;line-height:40px}</style></head><body><div class='container'><h1>Device Portal</h1><div class='btn'><a href='/deviceSetup'>Device Setup </a></div><div class='btn'><a href='/currentConfigs'>Configurations </a></div><div class='btn'><a href='/reboot'>Reboot </a></div></div></body></html>";
    String page = FPSTR(INDEX);
    server->send(200, "text/html", page);
}

void ConfigServer::handleDeviceSetup() {
    const char DEVICE_SETUP_PAGE[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Device Setup</title><style>html{box-sizing:border-box;font-size:16px}*,*:before,*:after{box-sizing:inherit}body,h1,h2,h3,h4,h5,h6,p,ol,ul{margin:0;padding:0;font-weight:400}ol,ul{list-style:none}body{font-family:Arial,Helvetica,sans-serif;margin-top:10px}img{max-width:100%;height:auto}.container{width:90%;max-width:450px;margin:0 auto}h1{text-align:center}input{display:block;width:100%;height:30px;margin-top:5px}.form-field{margin-top:10px}button[type='submit']{margin-top:10px;width:50px;height:30px;border-style:none;border-radius:3px;background-color:rgb(25,25,25);color:#fff}</style></head><body><div class='container'><h1>Device Setup</h1><form action='/saveConfig' method='POST'><div class='form-field'><label for='ssid'>SSID</label><input type='text' value='{{ssid}}' name='ssid' /></div><div class='form-field'><label for='password'>Password</label><input type='text' value='{{password}}' name='password' /></div><div class='form-field'><label for='tankHeight'>Tank Height (cm)</label><input type='number' value='{{tankHeight}}' name='tankHeight' /></div><div class='form-field'><label for='tankCircumference'>Tank Circumference (cm)</label><input type='number' value='{{tankCircumference}}' name='tankCircumference' /></div><button type='submit'>Save</button></form></div></body></html>";

    String page = FPSTR(DEVICE_SETUP_PAGE);

    page.replace("{{ssid}}", configDoc["ssid"].as<String>());
    page.replace("{{password}}", configDoc["password"].as<String>());
    page.replace("{{tankHeight}}", configDoc["tankHeight"].as<String>());
    page.replace("{{tankCircumference}}", configDoc["tankCircumference"].as<String>());

    server->send(200, "text/html", page);
}

void ConfigServer::handleSaveConfig() {
    if(server->hasArg("ssid")) {
        Serial.println(server->arg("ssid"));
        configDoc["ssid"] = server->arg("ssid");
    }

    if(server->hasArg("password")) {
        Serial.println(server->arg("password"));
        configDoc["password"] = server->arg("password");
    }

    if(server->hasArg("tankHeight")) {
        Serial.println(server->arg("tankHeight"));
        configDoc["tankHeight"] = server->arg("tankHeight");
    }

    if(server->hasArg("tankCircumference")) {
        Serial.println(server->arg("tankCircumference"));
        configDoc["tankCircumference"] = server->arg("tankCircumference");
    }

    File configFile = SPIFFS.open("/config.json", "w");

    if (!configFile) {
        Serial.println("Failed to open config file for writing");
    }
    
    serializeJson(configDoc, configFile);

    configFile.close();

    const char SAVE_CONFIG_PAGE[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Success</title><style>body{font-family:Arial,Helvetica,sans-serif;margin-top:10px}</style></head><body><h1>Configurations saved please restart the device.</h1></body></html>";

    String page = FPSTR(SAVE_CONFIG_PAGE);
    server->send(200, "text/html", page);
}

void ConfigServer::handleCurrentConfigs() {
    const char CURRENT_CONFIG_HTML[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Configurations</title><style>html{box-sizing:border-box;font-size:16px}*,*:before,*:after{box-sizing:inherit}body,h1,h2,h3,h4,h5,h6,p,ol,ul{margin:0;padding:0;font-weight:400}ol,ul{list-style:none}body{font-family:Arial,Helvetica,sans-serif;margin-top:10px}img{max-width:100%;height:auto}.container{width:90%;max-width:450px;margin:0 auto}h1{text-align:center}table{border-collapse:collapse;width:100%;margin-top:20px}td,th{border:1px solid #ddd;text-align:left;padding:8px}</style></head><body><div class='container'><h1>Configurations</h1><table><tr><td>SSID</td><td>{{ssid}}</td></tr><tr><td>Password</td><td>{{password}}</td></tr><tr><td>Tank Height</td><td>{{tankHeight}}</td></tr><tr><td>Tank Circumference</td><td>{{tankCircumference}}</td></tr></table></div></body></html>";

    String page = FPSTR(CURRENT_CONFIG_HTML);

    page.replace("{{ssid}}", configDoc["ssid"].as<String>());
    page.replace("{{password}}", configDoc["password"].as<String>());
    page.replace("{{tankHeight}}", configDoc["tankHeight"].as<String>());
    page.replace("{{tankCircumference}}", configDoc["tankCircumference"].as<String>());

    server->send(200, "text/html", page);
}

void ConfigServer::handleReboot() {
    Serial.println("Restarting esp in 10 seconds");
    delay(5000);
    ESP.restart();
}

ConfigServer::~ConfigServer(){}