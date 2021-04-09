#ifndef _GEOMETRY_ICOSAHEDRON_H_
#define _GEOMETRY_ICOSAHEDRON_H_
#include <Adafruit_NeoPixel.h>

#define LED_META (NEO_GRBW + NEO_KHZ800)
#define GEOMETRY_WIDTH 20
#define GEOMETRY_TYPE "icosahedron"

static const char hardware_map[20] = {
  0,  // 1
  4,  // 2
  5,  // 3
  6,  // 4
  7,  // 5
  2,  // 6
  3,  // 7
  18, // 8
  19, // 9
  14, // 10
  13, // 11
  12, // 12
  8,  // 13
  9, // 14
  1,  // 15
  16, // 16
  17, // 17
  15, // 18
  11, // 19
  10  // 20
};

#endif
