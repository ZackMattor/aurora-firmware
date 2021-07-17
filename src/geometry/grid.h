#ifndef _GEOMETRY_GRID_H_
#define _GEOMETRY_GRID_H_
#include <Adafruit_NeoPixel.h>

#define LED_META (NEO_GRB + NEO_KHZ800)
#define GEOMETRY_WIDTH 16

const String geometry_name = "grid";

static const char hardware_map[16] = {
  15,
  8,
  7,
  0,
  14,
  9,
  6,
  1,
  13,
  10,
  5,
  2,
  12,
  11,
  4,
  3
};

#endif
