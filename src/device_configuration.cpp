#include "SPIFFS.h"

typedef struct device_configuration
{
  char mqtt_server[128];
  int mqtt_port;

  char device_token[16];
} device_configuration;

int load_settings()
{
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return -1;
  }

  File id_file = SPIFFS.open("/config/id.txt");
  File mqtt_file = SPIFFS.open("/config/mqtt.txt");

  device_token = id_file.readStringUntil('\n');
  mqtt_server = mqtt_file.readStringUntil('\n');

  strncpy(settings.mqtt_server, mqtt_server.c_str(), 128);
  strncpy(settings.device_token, device_token.c_str(), 16);
  settings.mqtt_port = 1337;
}
