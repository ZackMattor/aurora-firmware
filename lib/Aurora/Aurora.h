#ifndef _AURORA_H_
#define _AURORA_H_

#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>

//struct PwmChain {
//  int pin;
//};
//
//struct PwmChain {
//  int pin;
//};

//LinkedList<Adafruit_NeoPixel> * neo_pixel_chains = new LinkedList<String>();
//LinkedList<> * pwm_chains = new LinkedList<String>();

void aurora_init(String endpoint, String id);
void aurora_sendTelemetry(void);
void aurora_connect(void);
void aurora_check_connection(void);
void aurora_process(void);
int aurora_render(const char hardware_map[]);
void aurora_add_output_neo(int width, int pin, int led_meta, String geometry);

#endif
