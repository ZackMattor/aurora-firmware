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

/** ICOSAHEDRON MAPPINGS **/
//static const char icosahedron_neighbors[20][3] = {
//  { 15,  2, 5 },    // 1
//  { 7, 3, 1 },      // 2
//  { 2, 9, 4 },      // 3
//  { 5, 3, 11 },     // 4
//  { 1, 4, 13 },     // 5
//  { 16,  7, 15 },   // 6
//  { 6, 8, 2 },      // 7
//  { 7, 17,  9 },    // 8
//  { 8, 10,  3 },    // 9
//  { 9, 18,  11 },   // 10
//  { 4, 10,  12 },   // 11
//  { 13,  11,  19 }, // 12 
//  { 14,  5, 12 },   // 13
//  { 20,  15,  13 }, // 14
//  { 6, 1, 14 },     // 15
//  { 20,  17,  6 },  // 16
//  { 16,  18,  8 },  // 17
//  { 10,  17,  19 }, // 18
//  { 20,  12,  18 }, // 19
//  { 16,  14,  19 }  // 20
//};

#endif
