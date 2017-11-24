//
//#include "shelf.h"
//#include <Adafruit_NeoPixel.h>
//
//Adafruit_NeoPixel shelves[5];
//int colors[4];
//
//void shelf_int() {
//  // Initialize neopixel strands
//  shelves[0] = Adafruit_NeoPixel(LEDS_PER_SHELF, SHELF_0_PIN, NEO_GRBW + NEO_KHZ800);
//  shelves[1] = Adafruit_NeoPixel(LEDS_PER_SHELF, SHELF_1_PIN, NEO_GRBW + NEO_KHZ800);
//  shelves[2] = Adafruit_NeoPixel(LEDS_PER_SHELF, SHELF_2_PIN, NEO_GRBW + NEO_KHZ800);
//  shelves[3] = Adafruit_NeoPixel(LEDS_PER_SHELF, SHELF_3_PIN, NEO_GRBW + NEO_KHZ800);
//  shelves[4] = Adafruit_NeoPixel(LEDS_PER_SHELF, SHELF_4_PIN, NEO_GRBW + NEO_KHZ800);
//
//  // Some colors to play with
//  colors[0] = shelves[0].Color(200,0,0);
//  colors[1] = shelves[0].Color(100,100,0);
//  colors[2] = shelves[0].Color(0,200,0);
//  colors[3] = shelves[0].Color(0,100,100);
//  colors[4] = shelves[0].Color(0,0,200);
//  colors[5] = shelves[0].Color(100,0,100);
//
//  // Initialize all shelves
//  for(int y=0; y<NUMBER_OF_SHELVES; y++) {
//    shelves[y].begin();
//  }
//}
//
//void shelf_clear_all() {
//  for(int y=0; y<NUMBER_OF_SHELVES; y++) {
//    shelves[y].clear();
//    shelves[y].show();
//  }
//}
//
//void white_swell(int frame) {
//  for(int y=0;y<NUMBER_OF_SHELVES;y++) {
//    for(int x=0;x<LEDS_PER_SHELF;x++) {
//      shelves[y].setPixelColor(x, shelves[0].Color(0,4*(frame % 62),0,0));
//    }
//    shelves[y].show();
//  }
//}
//
//void color_walk(int frame) {
//  int position = frame % LEDS_PER_SHELF;
//  int color_id = ( frame / LEDS_PER_SHELF ) % NUMCOLORS;//frame % NUMCOLORS;
//
//  for(int y=0;y<NUMBER_OF_SHELVES;y++) {
//    for(int x=0;x<LEDS_PER_SHELF;x++) {
//      if(position == x) shelves[y].setPixelColor(x, colors[color_id]);
//      else shelves[y].setPixelColor(x, shelves[0].Color(0,0,0));
//    }
//    shelves[y].show();
//  }
//}
//
//void color_sweep(int frame) {
//  int new_color_id = (frame/LEDS_PER_SHELF) % NUMCOLORS;
//  int old_color_id = (new_color_id+NUMCOLORS-1) % NUMCOLORS;
//  int cur_position = frame % LEDS_PER_SHELF;
//
//  for(int y=0;y<NUMBER_OF_SHELVES;y++) {
//    for(int x=0;x<LEDS_PER_SHELF;x++) {
//      if(cur_position < x) shelves[y].setPixelColor(x, colors[old_color_id]);
//      else shelves[y].setPixelColor(x, colors[new_color_id]);
//    }
//    shelves[y].show();
//  }
//}
//
//void dot_runner(int frame) {
//  int steps = 10;
//  int total_steps = steps * LEDS_PER_SHELF;
//
//  int animation_frame = frame % total_steps;
//  int col_position = animation_frame/steps % 8;
//  float aa = (animation_frame - col_position*steps) / steps;
//
//  for(int y=0;y<NUMBER_OF_SHELVES;y++) {
//    for(int x=0;x<LEDS_PER_SHELF;x++) {
//      if(col_position ==  x) shelves[y].setPixelColor(x, 200 * aa, 0, 0,0);
//      else shelves[y].setPixelColor(x, 0,0,20,0);
//    }
//    shelves[y].show();
//  }
//}
