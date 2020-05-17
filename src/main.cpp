#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "QList.h"
#include "config.h"
#include "geometry/icosahedron.h"
#include "ota_update.h"

const char* ssid        = CLIENT_SSID;
const char* password    = CLIENT_PASSPHRASE;
const char* mqtt_server = MQTT_SERVER;
const String geometry = "icosahedron";
String device_id;

Adafruit_NeoPixel *led_strip = new Adafruit_NeoPixel(GEOMETRY_WIDTH, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

QList<byte *> frame_buffer;
unsigned int frame_count = 0;
unsigned long current_time = 0;

// hacky clock system variables
unsigned long telemetry_interval = 5000;
unsigned long next_telemetry_time = telemetry_interval;

unsigned long framereport_interval = 1000;
unsigned long next_framereport_time = framereport_interval;

unsigned long rendertik_interval = 1000 / 40;
unsigned long next_rendertik_time = rendertik_interval;

void callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic, (device_id + "_ff").c_str()) == 0 && length%3 == 0) {
    frame_count++;
    if(frame_buffer.size() < 500) {
      byte *cpy = (byte *)malloc(length+1);
      memcpy(cpy, payload, length);
      frame_buffer.push_back(cpy);
    }
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

  ota_setup();

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  led_strip->begin();
  led_strip->show(); // Initialize all pixels to 'off'

  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);
}

void loop() {
  ArduinoOTA.handle();
  current_time = millis();

  if (!mqtt_client.connected()) {
    reconnect();
  }

  mqtt_client.loop();

  // Clock for the render tik
  if(current_time > next_rendertik_time && frame_buffer.size() > 80) {
    next_rendertik_time = current_time + rendertik_interval;

    unsigned int length = GEOMETRY_WIDTH * 3;

    byte *payload = frame_buffer.front();
    frame_buffer.pop_front();
    unsigned int led_id;

    for(int i = 0; i < length; i+=3) {
      led_id = i/3;
      //row_id = (i/3)%20;

      led_strip->setPixelColor(icosahedron_hardware_map[led_id], led_strip->Color(payload[i], payload[i+1], payload[i+2], 0));
    }

    free(payload);
    led_strip->show();
  }

  // Clock for the frame repot
  if(current_time > next_framereport_time) {
    next_framereport_time = current_time + framereport_interval;

    Serial.print("Frame Buffer Size - ");
    Serial.println(frame_buffer.size());

    //Serial.println(frame_count);
    frame_count = 0;
  }

  // Clock for the telemetry sender
  if(current_time > next_telemetry_time) {
    next_telemetry_time = current_time + telemetry_interval;

    //Serial.print(String("Sending telemetry...") + millis() + " | ");
    sendTelemetry();
    //Serial.println(millis());
    //Serial.print("DEVICE ID: ");
    //Serial.print(device_id);
    //Serial.print(" | IP: ");
    //Serial.println(WiFi.localIP());
  }
}
