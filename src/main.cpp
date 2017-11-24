//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>
#include <Arduino.h>

#include "shelf.h"
#include "animation.h"

const char* ssid     = "foobar";
const char* password = "ApplesAreGoodForYou";

int frame = 0;
int time=0;
int last_frame=0;
int frame_interval=50;

Shelf *shelf;
Animation *animation;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);

  //while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //  Serial.println("Connection Failed! Rebooting...");
  //  delay(5000);
  //  ESP.restart();
  //}

  //ArduinoOTA.onStart([]() {
  //    String type;
  //    if (ArduinoOTA.getCommand() == U_FLASH)
  //    type = "sketch";
  //    else // U_SPIFFS
  //    type = "filesystem";

  //    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  //    Serial.println("Start updating " + type);
  //    });
  //ArduinoOTA.onEnd([]() {
  //    Serial.println("\nEnd");
  //    });
  //ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //    });
  //ArduinoOTA.onError([](ota_error_t error) {
  //    Serial.printf("Error[%u]: ", error);
  //    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //    });
  //ArduinoOTA.begin();
  //Serial.println("Ready");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  shelf = new Shelf();
  animation = new Animation(shelf);
}


void loop() {
  //ArduinoOTA.handle();

  if(last_frame + frame_interval < time) {
    animation->color_walk(frame);

    frame++;
    last_frame = time;
  }

  time = millis();
}
