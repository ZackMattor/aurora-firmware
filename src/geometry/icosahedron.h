#ifndef _GEOMETRY_ICOSAHEDRON_H_
#define _GEOMETRY_ICOSAHEDRON_H_
#include <Adafruit_NeoPixel.h>

#define LED_META (NEO_GRBW + NEO_KHZ800)
#define GEOMETRY_WIDTH 20

const String geometry_name = "icosahedron";

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
