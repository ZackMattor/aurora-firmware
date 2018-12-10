#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include "SPIFFS.h"

Adafruit_NeoPixel *led_strip;
String mqtt_server;
String id_str;

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  bool res;

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if(!SPIFFS.exists("/config/mqtt.txt")) Serial.println("There is no valid mqtt config");
  if(!SPIFFS.exists("/config/id.txt")) Serial.println("There is no valid id config");

  File id_file = SPIFFS.open("/config/id.txt");
  File mqtt_file = SPIFFS.open("/config/mqtt.txt");

  Serial.write("ID Value:");
  id_str = id_file.readStringUntil('\n');
  Serial.println(id_str.c_str());

  Serial.write("MQTT Value:");
  mqtt_server = mqtt_file.readStringUntil('\n');
  Serial.println(mqtt_server.c_str());

  //File file = SPIFFS.open("/test.txt", FILE_WRITE);

  //if(!file) {
  //  Serial.println("There was an error opening the file for writing");
  //  return;
  //}

  //if(file.print("TEST")) {
  //  Serial.println("File was written");;
  //} else {
  //  Serial.println("File write failed");
  //}

  //file.close();
  //

  //File file2 = SPIFFS.open("/test.txt");

  //if(!file2) {
  //  Serial.println("Failed to open file for reading");
  //  return;
  //}

  //Serial.println("File Content:");

  //while(file2.available()){
  //  Serial.write(file2.read());
  //}

  //file2.close();

  WiFiManagerParameter custom_mqtt_server("mqtt_server", "mqtt server", "", 40);
  wm.addParameter(&custom_mqtt_server);
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
