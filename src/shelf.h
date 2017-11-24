#ifndef _SHELF_H_
#define _SHELF_H_

#include <Adafruit_NeoPixel.h>

class Shelf {
  public:
    Shelf();
    void clear();
    void set_pixel(short shelf_id, short pixel_id, int color);
    void set_pixel(short shelf_id, short pixel_id, int r, int g, int b, int w);
    void render();

  private:
    int number_of_shelves;
    int leds_per_shelf;
    Adafruit_NeoPixel shelves[5];
};

//#define NUMBER_OF_SHELVES 5
//#define LEDS_PER_SHELF 20
//#define NUMCOLORS 6
//
//// Shelf pin numbers...
//// shelf 0 is on the bottom
//// shelf 4 is on the top
//#define SHELF_4_PIN 0
//#define SHELF_3_PIN 2
//#define SHELF_2_PIN 5
//#define SHELF_1_PIN 12
//#define SHELF_0_PIN 13
//
//
//void shelf_init();
//void shelf_clear_all();

#endif
