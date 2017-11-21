#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NUMCOLS 20
#define NUMROWS 5
#define NUMCOLORS 6

const char* ssid     = "foobar";
const char* password = "ApplesAreGoodForYou";

Adafruit_NeoPixel rows[5];

int frame = 0;
int colors[4];
int time=0;
int last_frame=0;
int frame_interval=50;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

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

  rows[0] = Adafruit_NeoPixel(NUMCOLS, 5, NEO_GRBW + NEO_KHZ800);
  rows[1] = Adafruit_NeoPixel(NUMCOLS, 2, NEO_GRBW + NEO_KHZ800);
  rows[2] = Adafruit_NeoPixel(NUMCOLS, 0, NEO_GRBW + NEO_KHZ800);
  rows[3] = Adafruit_NeoPixel(NUMCOLS, 13, NEO_GRBW + NEO_KHZ800);
  rows[4] = Adafruit_NeoPixel(NUMCOLS, 12, NEO_GRBW + NEO_KHZ800);

  delay(100);
  for(int y=0;y<NUMROWS;y++) {
    rows[y].begin();
  }

  colors[0] = rows[0].Color(200,0,0);
  colors[1] = rows[0].Color(100,100,0);
  colors[2] = rows[0].Color(0,200,0);
  colors[3] = rows[0].Color(0,100,100);
  colors[4] = rows[0].Color(0,0,200);
  colors[5] = rows[0].Color(100,0,100);
}

void white_swell(int frame) {
  for(int y=0;y<NUMROWS;y++) {
    for(int x=0;x<NUMCOLS;x++) {
      rows[y].setPixelColor(x, rows[0].Color(0,4*(frame % 62),0,0));
    }
    rows[y].show();
  }
}

void color_walk(int frame) {
  int position = frame % NUMCOLS;
  int color_id = ( frame / NUMCOLS ) % NUMCOLORS;//frame % NUMCOLORS;

  for(int y=0;y<NUMROWS;y++) {
    for(int x=0;x<NUMCOLS;x++) {
      if(position == x) rows[y].setPixelColor(x, colors[color_id]);
      else rows[y].setPixelColor(x, rows[0].Color(0,0,0));
    }
    rows[y].show();
  }
}

void color_sweep(int frame) {
  int new_color_id = (frame/NUMCOLS) % NUMCOLORS;
  int old_color_id = (new_color_id+NUMCOLORS-1) % NUMCOLORS;
  int cur_position = frame % NUMCOLS;

  for(int y=0;y<NUMROWS;y++) {
    for(int x=0;x<NUMCOLS;x++) {
      if(cur_position < x) rows[y].setPixelColor(x, colors[old_color_id]);
      else rows[y].setPixelColor(x, colors[new_color_id]);
    }
    rows[y].show();
  }
}

void dot_runner(int frame) {
  Serial.print("HI");
  int steps = 10;
  int total_steps = steps * NUMCOLS;

  int animation_frame = frame % total_steps;
  int col_position = animation_frame/steps % 8;
  float aa = (animation_frame - col_position*steps) / steps;
  Serial.print(aa);

  for(int y=0;y<NUMROWS;y++) {
    for(int x=0;x<NUMCOLS;x++) {
      if(col_position ==  x) rows[y].setPixelColor(x, 200 * aa, 0, 0,0);
      else rows[y].setPixelColor(x, 0,0,20,0);
    }
    rows[y].show();
  }
}

void loop() {
  ArduinoOTA.handle();

  //if(last_frame + frame_interval < time) {
  //  color_sweep(frame);
  //  frame++;
  //  last_frame = time;
  //}

  //time = millis();
}
