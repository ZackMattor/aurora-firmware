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
    int get_number_of_shelves();
    int get_leds_per_shelf();
    int color(int r, int g, int b, int w);
    int color(int r, int g, int b);

  private:
    int number_of_shelves;
    int leds_per_shelf;
    Adafruit_NeoPixel shelves[5];
};

#endif
