#include "Shelf.h"

#define SHELF_4_PIN 0
#define SHELF_3_PIN 2
#define SHELF_2_PIN 5
#define SHELF_1_PIN 12
#define SHELF_0_PIN 13

Shelf::Shelf() {
  // Set class variables
  number_of_shelves = 1;
  leds_per_shelf = 20;

  shelves[0] = Adafruit_NeoPixel(leds_per_shelf, SHELF_1_PIN, NEO_GRBW + NEO_KHZ800);
  // Initialize pixels
  //shelves[0] = Adafruit_NeoPixel(leds_per_shelf, SHELF_0_PIN, NEO_GRBW + NEO_KHZ800);
  //shelves[1] = Adafruit_NeoPixel(leds_per_shelf, SHELF_1_PIN, NEO_GRBW + NEO_KHZ800);
  //shelves[2] = Adafruit_NeoPixel(leds_per_shelf, SHELF_2_PIN, NEO_GRBW + NEO_KHZ800);
  //shelves[3] = Adafruit_NeoPixel(leds_per_shelf, SHELF_3_PIN, NEO_GRBW + NEO_KHZ800);
  //shelves[4] = Adafruit_NeoPixel(leds_per_shelf, SHELF_4_PIN, NEO_GRBW + NEO_KHZ800);

  for(int y = 0; y < number_of_shelves; y++) {
    Serial.print("Starting up a shelf LED strip\n");
    shelves[y].begin();
  }

  clear();
}

void Shelf::clear() {
  for(int y = 0; y < number_of_shelves; y++) {
    shelves[y].clear();
    shelves[y].show();
  }
}

void Shelf::set_pixel(int shelf_id, int pixel_id, int color) {
  Serial.print(shelf_id);
  Serial.print(" - ");
  Serial.print(pixel_id);
  Serial.print(" - ");
  Serial.print(color);
  Serial.print(" - ");
  Serial.print((int)&shelves[0], HEX);
  shelves[0].setPixelColor(pixel_id, color);
  Serial.print(" - AFTER!!\n");
}

void Shelf::set_pixel(int shelf_id, int pixel_id, int r, int g, int b, int w) {
  set_pixel(shelf_id, pixel_id, shelves[0].Color(r,g,b,w));
}

void Shelf::render() {
  for(int y = 0; y < number_of_shelves; y++) {
    shelves[y].show();
  }
}

int Shelf::get_number_of_shelves() {
  return number_of_shelves;
}

int Shelf::get_leds_per_shelf() {
  return leds_per_shelf;
}

int Shelf::color(int r, int g, int b) {
  return color(r,b,g,0);
}

int Shelf::color(int r, int g, int b, int w) {
  return shelves[0].Color(r,b,g,w);
}
