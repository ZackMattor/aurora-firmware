#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "QList.h"
#include "config.h"
#include "ota_update.h"

//#include "geometry/shard.h"
#include "geometry/icosahedron.h"

// WiFi/Network Settings
const char* ssid        = CLIENT_SSID;
const char* password    = CLIENT_PASSPHRASE;
const char* mqtt_server = MQTT_SERVER;
WiFiClient server_client;

// Aurora/LED Variables
String device_id;
const String geometry = GEOMETRY_TYPE;
Adafruit_NeoPixel *led_strip = new Adafruit_NeoPixel(GEOMETRY_WIDTH, NEOPIXEL_PIN, LED_META);

const unsigned int FRAME_BUFFER_SIZE = GEOMETRY_WIDTH * 3;
char frame_buffer[FRAME_BUFFER_SIZE];

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
  for(int x=0; x<GEOMETRY_WIDTH; x++) {
    //led_strip->setPixelColor(x, led_strip->ColorRGB(h,s,v));
    led_strip->setPixelColor(hardware_map[x], 0,0,0);
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

  memset(frame_buffer, 0, FRAME_BUFFER_SIZE);

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

short buffer_started = 0;
short buffer_ready = 0;


// 1 255 255 1 255 255 0 1 255 255 1 255 255 0 1 255 255 1 255 255 0

void loop() {
  // Transform data stream into frame buffer
  while(server_client.available()) {
    unsigned int buffer_length = strlen(frame_buffer);
    char c = server_client.read();
    Serial.print((byte)c);

    // The end of the old and start of the new!
    if(c == 0) {
      buffer_started = 1;

      if(buffer_length != 0) {
        buffer_ready = 1;
        break;
      }
    } else {
      if(buffer_started == 1 && buffer_length < FRAME_BUFFER_SIZE) {
        // Push item onto frame buffer
        strncat(frame_buffer, &c, 1);
      }
    }
  }

  if(buffer_ready == 1) {
    Serial.println("");
    Serial.println("Buffer Ready");
    Serial.println("");

    for(int i=0; i<strlen(frame_buffer); i++) {
      Serial.print((byte)frame_buffer[i]);
      Serial.print("|");
    }
    Serial.println("|");

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i=i+3) {
      unsigned int led_id = i / 3;

      if(led_id <= GEOMETRY_WIDTH) {
        led_strip->setPixelColor(led_id, led_strip->Color(frame_buffer[i], frame_buffer[i+1], frame_buffer[i+2]));
      }
    }

    //frames++;
    led_strip->show();

    buffer_ready = 0;
    memset(frame_buffer, 0, FRAME_BUFFER_SIZE);
  }
}
