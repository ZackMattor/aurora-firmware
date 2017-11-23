#ifdef _SHELF_H_
#define _SHELF_H_

#include <Adafruit_NeoPixel.h>

#define NUMBER_OF_SHELVES 5
#define LEDS_PER_SHELF 20

// Shelf pin numbers...
// shelf 0 is on the bottom
// shelf 4 is on the top
#define SHELF_4_PIN 0
#define SHELF_3_PIN 2
#define SHELF_2_PIN 5
#define SHELF_1_PIN 12
#define SHELF_0_PIN 13


void shelf_init();
void shelf_clear_all();

#endif
