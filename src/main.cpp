#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>
#include <Aurora.h>
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

Timer *reconnect_timer = timer_create(15000);
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

  ota_setup();

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to WiFi failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  aurora_add_output_neo(GEOMETRY_WIDTH, NEOPIXEL_PIN, LED_META, geometry_name);
  aurora_init(server_endpoint, String(WiFi.macAddress()));

  aurora_connect();
}

void loop() {
  // Read data from our TCP socket into our frame buffer.
  aurora_process();

  if(timer_check(frame_render_timer)) {
    frame_render_timer->next_tick += aurora_render(hardware_map);
  }

  if(timer_check(reconnect_timer)) {
    aurora_check_connection();
  }

  timer_tick();
}
