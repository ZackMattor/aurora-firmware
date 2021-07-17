#include "Aurora.h"

static WiFiClient aurora_client;
static String aurora_endpoint;
static String aurora_id;
static int drift_adjustment = 0;

static struct AuroraNeoOutput {
  Adafruit_NeoPixel *led_strip;
  unsigned int frame_buffer_size;
  LinkedList<String> * frame_buffer;
  int geometry_width;
  String geometry_name;
} aurora_neo_output;

// Inputs only support switches at the moment
enum AuroraInputType { AuroraSwitch };

static struct AuroraInput {
  int pin;
  String name;
  enum AuroraInputType type;
} aurora_input;

static LinkedList<AuroraNeoOutput> aurora_neo_outputs;
static LinkedList<AuroraInput> aurora_inputs;

void aurora_init(String endpoint, String id) {
  aurora_endpoint = endpoint;
  aurora_id = id;
}

void aurora_add_output_neo(int width, int pin, int led_meta, String geometry) {
  AuroraNeoOutput neo_output;

  neo_output.led_strip = new Adafruit_NeoPixel(width, pin, led_meta);
  neo_output.frame_buffer_size = width * 3;
  neo_output.frame_buffer = new LinkedList<String>();
  neo_output.geometry_width = width;
  neo_output.geometry_name = geometry;

  neo_output.led_strip->begin();
  neo_output.led_strip->show();

  aurora_neo_outputs.add(neo_output);
}

void aurora_add_input_switch(int pin, String name) {
  AuroraInput aurora_input;

  aurora_input.pin = pin;
  aurora_input.name = name;
  aurora_input.type = AuroraSwitch;


  // TODO - init pin, pullup?
  // TODO - Add all input state to device telemetry

  aurora_inputs.add(aurora_input);
}

void aurora_send_activate() {
  if(aurora_client.connected()) {
    const String activate_payload =
      String("{\"topic\": \"device_activate\", \"payload\": {") +
        "\"device_id\":\"" + aurora_id + "\"," +
        "\"geometry\":\"" + aurora_neo_outputs.get(0).geometry_name + "\"" +
      "}}";

    aurora_client.print(activate_payload.c_str());
  }
}

void aurora_send_telemetry() {
  if(aurora_client.connected()) {
    const String telemetry_payload =
      String("{\"topic\": \"device_telemetry\", \"payload\": {") +
        "\"device_id\":\"" + aurora_id + "\"," +
        "\"input_state\": []" +
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

  aurora_send_activate();
  Serial.println("Activate sent!");
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
  LinkedList<String> * frame_buffer = aurora_neo_outputs.get(0).frame_buffer;

  while(aurora_client.available()) {
    String line = aurora_client.readStringUntil(0);
    frame_buffer->add(line);

    break;
  }
}

int aurora_render(const char hardware_map[]) {
  // TODO - iterate over these
  AuroraNeoOutput neo_output = aurora_neo_outputs.get(0);

  Adafruit_NeoPixel *led_strip = neo_output.led_strip;
  unsigned int frame_buffer_size = neo_output.frame_buffer_size;
  LinkedList<String> * frame_buffer = neo_output.frame_buffer;
  int geometry_width = neo_output.geometry_width;

  if(frame_buffer->size() > 0) {
    char frame_chars[frame_buffer_size+1];
    frame_buffer->shift().toCharArray(frame_chars, frame_buffer_size+1);

    for(short int i = 0; i < frame_buffer_size; i=i+3) {
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

