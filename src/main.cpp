#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <PubSubClient.h>

#include "config.h"
//#include "shelf.h"
//#include "animation.h"

const char* ssid        = CLIENT_SSID;
const char* password    = CLIENT_PASSPHRASE;
const char* mqtt_server = MQTT_SERVER;
const String geometry = "icosahedron";
String device_id;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

int frame = 0;

//Shelf *shelf;
//Animation *animation;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);
  int shelf_id;
  int row_id;

  if(strcmp(topic, "ff") == 0 && length%3 == 0) {
    for(int i = 0; i < length; i+=3) {
      shelf_id = i/60;
      row_id = (i/3)%20;

      //shelf->set_pixel(shelf_id, row_id, payload[i], payload[i+1], payload[i+2], 0);
    }

    //shelf->render();
  }
}

void sendTelemetry() {
  if(mqtt_client.connected()) {
    const String telemetry_payload =
      String("{") +
        "\"device_id\":\"" + device_id + "\"," +
        "\"geometry\":\"" + geometry + "\"" +
      "}";
    mqtt_client.publish("device_telemetry", telemetry_payload.c_str());
  }
}

void reconnect() {
  // Loop until we're reconnected
  while(!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "AuroraDevice-";
    clientId += device_id;

    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");

      mqtt_client.subscribe((device_id + "_ff").c_str());
      sendTelemetry();
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
      ArduinoOTA.handle();
      delay(1000);
      ArduinoOTA.handle();
      delay(1000);
      ArduinoOTA.handle();
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  device_id = String(WiFi.macAddress());
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";
    else // U_SPIFFS
    type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //shelf = new Shelf();
  //animation = new Animation(shelf);

  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);
}

void loop() {
  ArduinoOTA.handle();

  if( frame % 100000 == 0) {
    sendTelemetry();
    Serial.print("FRAME: ");
    Serial.print(frame);
    Serial.print(" | IP: ");
    Serial.println(WiFi.localIP());
  }

  if (!mqtt_client.connected()) {
    reconnect();
  }

  mqtt_client.loop();
  frame++;
}
