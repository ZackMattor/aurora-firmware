#include "SPIFFS.h"
#include "device_config.h"

typedef struct device_config
{
  char mqtt_server[128];
  int mqtt_port;

  char device_token[16];
} device_config;

static device_config settings;

int device_config_init()
{
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return -1;
  }

  File id_file = SPIFFS.open("/config/id.txt");
  File mqtt_file = SPIFFS.open("/config/mqtt.txt");

  String device_token = id_file.readStringUntil('\n');
  String mqtt_server = mqtt_file.readStringUntil('\n');

  strncpy(settings.mqtt_server, mqtt_server.c_str(), 128);
  strncpy(settings.device_token, device_token.c_str(), 16);
  settings.mqtt_port = 1337;

  id_file.close();
  mqtt_file.close();

  return 0;
}
