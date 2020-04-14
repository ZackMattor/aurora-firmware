#include "animation.h"

Animation::Animation(Shelf* a_shelf) {
  Serial.print("constructing animation");
  shelf = a_shelf;
  colors[0] = shelf->color(200,0,0);
  colors[1] = shelf->color(100,100,0);
  colors[2] = shelf->color(0,200,0);
  colors[3] = shelf->color(0,100,100);
  colors[4] = shelf->color(0,0,200);
  colors[5] = shelf->color(100,0,100);

  number_of_shelves = shelf->get_number_of_shelves();
  leds_per_shelf = shelf->get_leds_per_shelf();
}

void Animation::color_walk(int frame) {
  int position = frame % leds_per_shelf;
  int color_id = ( frame / leds_per_shelf ) % 5;//frame % NUMCOLORS;

  for(int y = 0; y < number_of_shelves; y++) {
    for(int x = 0; x < leds_per_shelf; x++) {
      if(position == x) {
        shelf->set_pixel(y, x, colors[color_id]);
      }
      else {
        shelf->set_pixel(y, x, 0, 0, 0, 0);
      }
    }
  }

  shelf->render();
}
