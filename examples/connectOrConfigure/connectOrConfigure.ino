#include <SPIFFS.h>
#include "ConfigManager.h"

void setup() {

  Serial.begin(115200);

  if(!SPIFFS.begin(true)) {
    Serial.println("Unable to start SPIFFS");
    return;
  }

  Serial.println("SPIFFS started");
  
  //SPIFFS.remove("/config.json");

  ConfigManager manager("test");  

  manager.connectOrConfigure();
}

void loop() {

}
