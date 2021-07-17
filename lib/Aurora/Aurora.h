#ifndef _AURORA_H_
#define _AURORA_H_

#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>

WiFiClient aurora_client;
String aurora_endpoint;
String aurora_geometry_name;
String aurora_id;
int drift_adjustment = 0;

Adafruit_NeoPixel *led_strip;
unsigned int FRAME_BUFFER_SIZE;
LinkedList<String> * frame_buffer;
int geometry_width;

//struct PwmChain {
//  int pin;
//};
//
//struct PwmChain {
//  int pin;
//};

//LinkedList<Adafruit_NeoPixel> * neo_pixel_chains = new LinkedList<String>();
//LinkedList<> * pwm_chains = new LinkedList<String>();

void aurora_init(String endpoint, String id) {
  frame_buffer = new LinkedList<String>();
  aurora_endpoint = endpoint;
  Serial.println(endpoint);
  aurora_id = id;

  led_strip->begin();
  led_strip->show(); // Initialize all pixels to 'off'
}

void aurora_sendTelemetry() {
  if(aurora_client.connected()) {
    const String telemetry_payload =
      String("{\"topic\": \"device_telemetry\", \"payload\": {") +
        "\"device_id\":\"" + aurora_id + "\"," +
        "\"geometry\":\"" + aurora_geometry_name + "\"" +
      "}}";

    aurora_client.print(telemetry_payload.c_str());
  }
}


void aurora_connect() {
  Serial.print("Attempting home hub connection...");

  char endpoint[aurora_endpoint.length()+1];
  aurora_endpoint.toCharArray(endpoint, aurora_endpoint.length()+1);
  Serial.println(endpoint);
  if (!aurora_client.connect(endpoint, 1337)) {
    Serial.println(aurora_endpoint);
    Serial.println("Connection to host failed! :(");
    return;
  }

  aurora_sendTelemetry();
  Serial.println("Telemetry sent!");
  Serial.println("Connection Successful");
}

void aurora_check_connection() {
  if(!aurora_client.connected()) {
    Serial.println(aurora_endpoint);
    Serial.println("connection gone.... attempting reconnect...");
    aurora_connect();
  }
}

void aurora_process() {
  while(aurora_client.available()) {
    String line = aurora_client.readStringUntil(0);
    frame_buffer->add(line);

    break;
  }
}

int aurora_render(const char hardware_map[]) {
  if(frame_buffer->size() > 0) {
    char frame_chars[FRAME_BUFFER_SIZE+1];
    frame_buffer->shift().toCharArray(frame_chars, FRAME_BUFFER_SIZE+1);

    for(short int i = 0; i < FRAME_BUFFER_SIZE; i=i+3) {
      unsigned int led_id = hardware_map[i / 3];

      if(led_id <= geometry_width) {
        led_strip->setPixelColor(led_id,
                                 led_strip->Color(frame_chars[i],
                                                  frame_chars[i+1],
                                                  frame_chars[i+2]));
      }
    }

    led_strip->show();

    // Keep our buffer between 5 and 10 in length
    if(frame_buffer->size() > 10) drift_adjustment = -5;
    else if(frame_buffer->size() < 5) drift_adjustment = 5;
    else drift_adjustment = 0;

    Serial.print(frame_buffer->size());
    Serial.print(" | ");
    Serial.print(drift_adjustment);
    Serial.println(" | Rendered");

    return drift_adjustment;
  }
  return 0;
}

void aurora_add_output_neo(int width, int pin, int led_meta, String geometry) {
  geometry_width = width;
  aurora_geometry_name = geometry;
  FRAME_BUFFER_SIZE = geometry_width * 3;
  led_strip = new Adafruit_NeoPixel(geometry_width, pin, led_meta);
}

#endif
