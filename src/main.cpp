#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include "QList.h"
#include "config.h"
#include "ota_update.h"
#include "utils/timer.h"

#if GEOMETRY_TYPE == GEOMETRY_SHARD
#include "geometry/shard.h"
#endif

#if GEOMETRY_TYPE == GEOMETRY_ICOSAHEDRON
#include "geometry/icosahedron.h"
#endif

#if GEOMETRY_TYPE == GEOMETRY_GRID
#include "geometry/grid.h"
#endif

// WiFi/Network Settings
const char* ssid            = CLIENT_SSID;
const char* password        = CLIENT_PASSPHRASE;
const char* server_endpoint = SERVER_ENDPOINT;
WiFiClient server_client;

// Aurora/LED Variables
Adafruit_NeoPixel *led_strip = new Adafruit_NeoPixel(GEOMETRY_WIDTH, NEOPIXEL_PIN, LED_META);

const unsigned int FRAME_BUFFER_SIZE = GEOMETRY_WIDTH * 3;

unsigned long loop_count = 0;
unsigned long current_time = 0;
unsigned int frames = 0;
int drift_adjustment = 0;

LinkedList<String> * frame_buffer = new LinkedList<String>();

Timer *reconnect_timer = timer_create(15000);
Timer *frame_render_timer = timer_create(1000 / 30);

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
        "\"device_id\":\"" + String(WiFi.macAddress()) + "\"," +
        "\"geometry\":\"" + geometry_name + "\"" +
      "}}";

    server_client.print(telemetry_payload.c_str());
    Serial.println("Telemetry sent!");
  }
}

void reconnectToAurora() {
  Serial.print("Attempting home hub connection...");

  if (!server_client.connect(server_endpoint, 1337)) {
    Serial.println("Connection to host failed! :(");
    return;
  }

  sendTelemetry();
  Serial.println("Connection Successful");
}

void wiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect to the access point...");
  WiFi.begin(ssid, password);
}

void setup() {
  delay(5000);

  Serial.begin(115200);
  Serial.println("Booting");

  WiFi.mode(WIFI_STA);
  WiFi.onEvent(wiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.begin(ssid, password);

  ota_setup();

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to WiFi failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  led_strip->begin();
  led_strip->show(); // Initialize all pixels to 'off'

  reconnectToAurora();
}

void loop() {
  // Read data from our TCP socket into our frame buffer.
  while(server_client.available()) {
    String line = server_client.readStringUntil(0);
    frame_buffer->add(line);

    break;
  }

  if(frame_buffer->size() > 0 && timer_check(frame_render_timer)) {
    char frame_chars[FRAME_BUFFER_SIZE+1];
    frame_buffer->shift().toCharArray(frame_chars, FRAME_BUFFER_SIZE+1);

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i=i+3) {
      unsigned int led_id = hardware_map[i / 3];

      if(led_id <= GEOMETRY_WIDTH) {
        led_strip->setPixelColor(led_id,
                                 led_strip->Color(frame_chars[i],
                                                  frame_chars[i+1],
                                                  frame_chars[i+2]));
      }
    }

    led_strip->show();
    frames++;

    // Keep our buffer between 5 and 10 in length
    if(frame_buffer->size() > 10) drift_adjustment = -5;
    else if(frame_buffer->size() < 5) drift_adjustment = 5;
    else drift_adjustment = 0;

    Serial.print(frame_buffer->size());
    Serial.print(" | ");
    Serial.print(drift_adjustment);
    Serial.println(" | Rendered");

    frame_render_timer->next_tick += drift_adjustment;
  }

  if(timer_check(reconnect_timer)) {
    if(!server_client.connected()) {
      Serial.println("connection gone.... attempting reconnect...");
      reconnectToAurora();
    }

  }

  timer_tick();
}
