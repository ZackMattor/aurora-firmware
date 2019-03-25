#include "SPIFFS.h"

#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include "device_config.h"

Adafruit_NeoPixel *led_strip;
String mqtt_server;
String id_str;

void setup() {
  Serial.begin(115200);
  bool res;

  device_config_init();

  WiFiManager wm;

  // Custom WiFiManager parameters
  WiFiManagerParameter param_mqtt_server("mqtt_server", "mqtt server", "", 128);
  WiFiManagerParameter param_mqtt_port("mqtt_port", "mqtt port", "1883", 8);
  WiFiManagerParameter param_device_token("device_token", "Device Token", "", 16);
  WiFiManagerParameter param_device_width("device_width", "LED Columns", "", 4);
  WiFiManagerParameter param_device_height("device_height", "LED Rows", "", 4);

  // Regester the custom parameters with WiFiManager
  wm.addParameter(&param_mqtt_server);
  wm.addParameter(&param_mqtt_port);
  wm.addParameter(&param_device_token);
  wm.addParameter(&param_device_width);
  wm.addParameter(&param_device_height);

  // Start the WiFi Manager workflow
  res = wm.autoConnect("Aurora", "ConfigAurora"); 

  if(!res) {
    Serial.println("Failed to connect");
    ESP.restart();
    delay(5000);
  } else {
    Serial.println("connected...yeey :)");
    led_strip = new Adafruit_NeoPixel((int)8, (int)5, NEO_GRBW + NEO_KHZ800);
    led_strip->begin();
    led_strip->clear();
  }
}

void wipe_data() {
  WiFi.disconnect();
  delay(1000);

  // https://github.com/espressif/arduino-esp32/issues/400
  WiFi.begin("0","0");

  ESP.restart();
  delay(4000);
}

void loop() {
  // PROVISION
  // launch the AP mode to configure the device
  // if we can contact the server, we can skip
  // the provisioning step
  //
  // ACTIVATE
  //
  //
  // LED animation loop
  wipe_data();
  int color = led_strip->Color(0, 0, 20, 0);

  led_strip->setPixelColor((int)0, color);
  led_strip->setPixelColor((int)1, color);
  led_strip->setPixelColor((int)2, color);
  led_strip->setPixelColor((int)3, color);
  led_strip->setPixelColor((int)4, color);
  led_strip->setPixelColor((int)5, color);
  led_strip->setPixelColor((int)6, color);
  led_strip->setPixelColor((int)7, color);
  led_strip->show();

  delay(1000);
}
