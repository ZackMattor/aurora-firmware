#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include "QList.h"
#include "config.h"
//#include "ota_update.h"

//#include "geometry/shard.h"
#include "geometry/icosahedron.h"

// WiFi/Network Settings
const char* ssid        = CLIENT_SSID;
const char* password    = CLIENT_PASSPHRASE;
const char* server_endpoint = MQTT_SERVER;
WiFiClient server_client;

// Aurora/LED Variables
String device_id;
const String geometry = GEOMETRY_TYPE;
Adafruit_NeoPixel *led_strip = new Adafruit_NeoPixel(GEOMETRY_WIDTH, NEOPIXEL_PIN, LED_META);

const unsigned int FRAME_BUFFER_SIZE = GEOMETRY_WIDTH * 3;

//unsigned short frame_writer_index = 0;
//unsigned short frame_render_index = 0;
//char frame_buffer[100][FRAME_BUFFER_SIZE];
LinkedList<String> * frame_buffer = new LinkedList<String>();

unsigned long loop_count = 0;
unsigned long current_time = 0;

// hacky clock system variables
//unsigned long telemetry_interval = 5000;
//unsigned long next_telemetry_time = telemetry_interval;

unsigned long rendertik_interval = 1000 / 30;
unsigned long next_rendertik_time = rendertik_interval;
int drift_adjustment = 0;

//unsigned long fps_interval = 1000;
//unsigned long next_fps_time = fps_interval;
unsigned int frames = 0;

//void clear(unsigned long h, short int s, short int v) {
//  for(int x=0; x<GEOMETRY_WIDTH; x++) {
//    //led_strip->setPixelColor(x, led_strip->ColorRGB(h,s,v));
//    led_strip->setPixelColor(hardware_map[x], 0,0,0);
//  }
//
//  led_strip->show();
//}

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

  if (!server_client.connect(server_endpoint, 1337)) {
    Serial.println("Connection to host failed! :(");
    return;
  }

  sendTelemetry();
  Serial.println("Connection to server worked! POG");
}

byte start_rendering = 0;

void setup() {
  delay(5000);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  device_id = String(WiFi.macAddress());
  WiFi.begin(ssid, password);

  //ota_setup();

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
  // Transform data stream into frame buffer
  while(server_client.available()) {
    String line = server_client.readStringUntil(0);
    frame_buffer->add(line);
    //Serial.print(frame_buffer->size());
    //Serial.println(" | New Frame");

    break;
  }

  if(next_rendertik_time < current_time && frame_buffer->size() > 0) {
    //for(int i=0; i<strlen(frame_buffer); i++) {
    //  Serial.print((byte)frame_buffer[i]);
    //  Serial.print("|");
    //}

    char frame_chars[FRAME_BUFFER_SIZE];
    frame_buffer->shift().toCharArray(frame_chars, FRAME_BUFFER_SIZE);

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i=i+3) {
      unsigned int led_id = i / 3;

      if(led_id <= GEOMETRY_WIDTH) {
        led_strip->setPixelColor(led_id, led_strip->Color(frame_chars[i], frame_chars[i+1], frame_chars[i+2]));
      }
    }

    led_strip->show();
    frames++;

    // Keep our buffer between 5 and 10 in length
    if(frame_buffer->size() > 10) drift_adjustment = -5;
    else if(frame_buffer->size() < 5) drift_adjustment = 5;
    else drift_adjustment = 0;

    //Serial.print(frame_buffer->size());
    //Serial.print(" | ");
    //Serial.print(drift_adjustment);
    //Serial.println(" | Rendered");

    next_rendertik_time = current_time + rendertik_interval + drift_adjustment;
  }

  current_time = millis();
}
