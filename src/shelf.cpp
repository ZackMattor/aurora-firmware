#include "Shelf.h"

#define SHELF_4_PIN 0
#define SHELF_3_PIN 2
#define SHELF_2_PIN 5
#define SHELF_1_PIN 12
#define SHELF_0_PIN 13

Shelf::Shelf() {
  // Set class variables
  number_of_shelves = 6;
  leds_per_shelf = 20;

  // Initialize pixels
  shelves[0] = Adafruit_NeoPixel(leds_per_shelf, SHELF_0_PIN, NEO_GRBW + NEO_KHZ800);
  shelves[1] = Adafruit_NeoPixel(leds_per_shelf, SHELF_1_PIN, NEO_GRBW + NEO_KHZ800);
  shelves[2] = Adafruit_NeoPixel(leds_per_shelf, SHELF_2_PIN, NEO_GRBW + NEO_KHZ800);
  shelves[3] = Adafruit_NeoPixel(leds_per_shelf, SHELF_3_PIN, NEO_GRBW + NEO_KHZ800);
  shelves[4] = Adafruit_NeoPixel(leds_per_shelf, SHELF_4_PIN, NEO_GRBW + NEO_KHZ800);

  for(int y = 0; y < number_of_shelves; y++) {
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

void Shelf::set_pixel(short shelf_id, short pixel_id, int color) {
  shelves[shelf_id].setPixelColor(pixel_id, color);
}

void Shelf::set_pixel(short shelf_id, short pixel_id, int r, int g, int b, int w) {
  set_pixel(shelf_id, pixel_id, shelves[0].Color(r,g,b,w));
}

void Shelf::render() {
  for(int y = 0; y < number_of_shelves; y++) {
    shelves[y].show();
  }
}
