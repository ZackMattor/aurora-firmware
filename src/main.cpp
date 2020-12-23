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

// WiFi/Network Settings
const char* ssid        = CLIENT_SSID;
const char* password    = CLIENT_PASSPHRASE;
const char* mqtt_server = MQTT_SERVER;
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Aurora/LED Variables
String device_id;
const String geometry = "icosahedron";
Adafruit_NeoPixel *led_strip = new Adafruit_NeoPixel(GEOMETRY_WIDTH, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

const unsigned int FRAME_BUFFER_SIZE = GEOMETRY_WIDTH * 3;
byte frame_buffer[FRAME_BUFFER_SIZE];

unsigned long loop_count = 0;
unsigned long current_time = 0;

// hacky clock system variables
unsigned long telemetry_interval = 5000;
unsigned long next_telemetry_time = telemetry_interval;

unsigned long rendertik_interval = 1000 / 40;
unsigned long next_rendertik_time = rendertik_interval;

void clear(unsigned long h, short int s, short int v) {
  for(int x=0; x<20; x++) {
    led_strip->setPixelColor(icosahedron_hardware_map[x], led_strip->ColorHSV(h,s,v));
  }

  led_strip->show();
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic, (device_id + "_ff").c_str()) == 0 && length%3 == 0) {
    memcpy(frame_buffer, payload, FRAME_BUFFER_SIZE);
  }
}

void sendTelemetry() {
  if(mqtt_client.connected()) {
    const String metrics_payload =
      String("{") +
        "\"alive\":1," +
        "\"free_heap\":" + ESP.getFreeHeap() +
      "}";
    const String telemetry_payload =
      String("{") +
        "\"device_id\":\"" + device_id + "\"," +
        "\"geometry\":\"" + geometry + "\"" +
      "}";

    mqtt_client.publish("device_telemetry", telemetry_payload.c_str());
    mqtt_client.publish("aurora_metrics/icosahedron", metrics_payload.c_str());
  }
}

void reconnect() {
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
  if(current_time > next_rendertik_time) {
    next_rendertik_time = current_time + rendertik_interval;

    clear(loop_count,255,255);

    led_strip->show();
  }

  // Clock for the telemetry sender
  if(current_time > next_telemetry_time) {
    next_telemetry_time = current_time + telemetry_interval;
    sendTelemetry();
  }
  loop_count++;
}
