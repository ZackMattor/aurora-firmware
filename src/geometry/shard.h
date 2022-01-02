#ifndef _GEOMETRY_SHARD_H_
#define _GEOMETRY_SHARD_H_
#include <Adafruit_NeoPixel.h>

#define LED_META (NEO_GRB + NEO_KHZ800)
#define GEOMETRY_WIDTH 6

const String geometry_name = "shard";

static const char hardware_map[7] = {
  0,
  1,
  2,
  3,
  4,
  5
};

#endif
