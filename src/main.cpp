#include <ESP8266WiFi.h>
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
WiFiClient server_client;

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

unsigned long fps_interval = 1000;
unsigned long next_fps_time = fps_interval;
unsigned int frames = 0;

void clear(unsigned long h, short int s, short int v) {
  for(int x=0; x<20; x++) {
    led_strip->setPixelColor(icosahedron_hardware_map[x], led_strip->ColorHSV(h,s,v));
  }

  led_strip->show();
}

void sendTelemetry() {
  if(server_client.connected()) {
    const String telemetry_payload =
      String("{\"topic\": \"device_telemetry\", \"payload\": {") +
        "\"device_id\":\"" + device_id + "\"," +
        "\"geometry\":\"" + geometry + "\"" +
      "}}";

    server_client.print(telemetry_payload.c_str());
    Serial.println("Telemetry sent!");
  }
}

void reconnect() {
  Serial.print("Attempting home hub connection...");
  // Create a random client ID
  String clientId = "AuroraDevice-";
  clientId += device_id;

  if (!server_client.connect(mqtt_server, 1337)) {
    Serial.println("Connection to host failed! :(");
    return;
  }

  sendTelemetry();
  Serial.println("Connection to server worked! POG");
}

void setup() {
  delay(5000);

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

  reconnect();
}

void loop() {
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi not connected...");
    ESP.restart();
  }

  if(server_client.available() >= FRAME_BUFFER_SIZE) {
    //Serial.println(server_client.available());

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i++) {
      char c = server_client.read();

      frame_buffer[i] = c;
    }

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i=i+3) {
      led_strip->setPixelColor(icosahedron_hardware_map[i%20], led_strip->Color(frame_buffer[i], frame_buffer[i+1], frame_buffer[i+2]));
    }
    frames++;
    led_strip->show();
  }

  ArduinoOTA.handle();
  current_time = millis();

  // Clock for the render tik
  if(current_time > next_rendertik_time) {
    next_rendertik_time = current_time + rendertik_interval;

    if(!server_client.connected()) {
      // strobe red when the server is not connected
      clear(0,255,((1+sin((float)loop_count / 20000)) * 255 / 4) + ((float)255*0.5));

      led_strip->show();
    } else {
    }
  }

  // Clock for the telemetry sender
  if(current_time > next_telemetry_time) {
    if(!server_client.connected()) {
      Serial.println("connection gone.... attempting reconnect...");
      reconnect();
    }

    next_telemetry_time = current_time + telemetry_interval;
  }

  // Clock for the telemetry sender
  if(current_time > next_fps_time) {
    Serial.println(frames);

    frames = 0;
    next_fps_time = current_time + fps_interval;
  }
  loop_count++;
}
