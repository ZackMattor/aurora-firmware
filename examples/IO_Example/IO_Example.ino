#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Aurora.h>
#include <AuroraTimer.h>
//#include "ota_update.h"

/** DEFINE THE GEOMETRY **/
/*************************/
#define LED_META (NEO_GRB + NEO_KHZ800)
#define NEOPIXEL_PIN 12
#define GEOMETRY_WIDTH 6

const String geometry_name = "shard";

static const char hardware_map[7] = {
  0,
  1,
  2,
  3,
  4,
  5
};
/*************************/

// WiFi/Network Settings
const char* ssid            = "My SSID";
const char* password        = "My SSID Password";
const char* server_endpoint = "10.0.0.20";

Timer *reconnect_timer = timer_create(15000);
Timer *telemetry_timer = timer_create(5000);
Timer *frame_render_timer = timer_create(1000 / 30);

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

  //ota_setup();

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to WiFi failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  aurora_add_output_neo(GEOMETRY_WIDTH, NEOPIXEL_PIN, LED_META, geometry_name);
  aurora_add_input_switch(33, String("center"));
  aurora_add_input_switch(32, String("left"));
  aurora_add_input_switch(25, String("right"));
  aurora_add_input_analog(34, String("pot"));
  aurora_init(server_endpoint, String(WiFi.macAddress()));

  aurora_connect();
}

void loop() {
  // Read data from our TCP socket into our frame buffer.
  aurora_process();

  if(timer_check(frame_render_timer)) {
    frame_render_timer->next_tick += aurora_render(hardware_map);
  }

  if(timer_check(telemetry_timer)) {
    aurora_send_telemetry();
  }

  if(timer_check(reconnect_timer)) {
    aurora_check_connection();
  }

  timer_tick();
}
