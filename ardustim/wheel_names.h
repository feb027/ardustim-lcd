/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Arbritrary wheel pattern generator short wheel names
 *
 * Provides shortened wheel names for LCD display
 * 
 * Part of Ardu-Stim LCD Interface Enhancement
 */
 
#ifndef __WHEEL_NAMES_H__
#define __WHEEL_NAMES_H__

#include <avr/pgmspace.h>

// Short wheel names for LCD display (max 20 chars)
const char short_wheel_name_0[] PROGMEM = "4-Cyl Dizzy";
const char short_wheel_name_1[] PROGMEM = "6-Cyl Dizzy";
const char short_wheel_name_2[] PROGMEM = "8-Cyl Dizzy";
const char short_wheel_name_3[] PROGMEM = "60-2 Crank";
const char short_wheel_name_4[] PROGMEM = "60-2 Crank+Cam";
const char short_wheel_name_5[] PROGMEM = "60-2 Half-Moon Cam";
const char short_wheel_name_6[] PROGMEM = "36-1 Crank";
const char short_wheel_name_7[] PROGMEM = "24-1 Crank";
const char short_wheel_name_8[] PROGMEM = "4-1 Crank+Cam";
const char short_wheel_name_9[] PROGMEM = "8-1 Crank (R6)";
const char short_wheel_name_10[] PROGMEM = "6-1 Crank+Cam";
const char short_wheel_name_11[] PROGMEM = "12-1 Crank+Cam";
const char short_wheel_name_12[] PROGMEM = "40-1 Ford V10";
const char short_wheel_name_13[] PROGMEM = "Dizzy 4-Cyl 50/40";
const char short_wheel_name_14[] PROGMEM = "Oddfire VR";
const char short_wheel_name_15[] PROGMEM = "36-2-2-2 H4 Crank";
const char short_wheel_name_16[] PROGMEM = "36-2-2-2 H6 Crank";
const char short_wheel_name_17[] PROGMEM = "36-2-2-2 Crank+Cam";
const char short_wheel_name_18[] PROGMEM = "GM LS1 Crank+Cam";
const char short_wheel_name_19[] PROGMEM = "GM 4-Tooth+Cam";
const char short_wheel_name_20[] PROGMEM = "GM 6-Tooth+Cam";
const char short_wheel_name_21[] PROGMEM = "GM 8-Tooth+Cam";
const char short_wheel_name_22[] PROGMEM = "12/1 Crank+Cam";
const char short_wheel_name_23[] PROGMEM = "24/1 Crank+Cam";
const char short_wheel_name_24[] PROGMEM = "Subaru 6/7";

// Array of pointers to short wheel names
const char* const short_wheel_names[] PROGMEM = {
  short_wheel_name_0,
  short_wheel_name_1,
  short_wheel_name_2,
  short_wheel_name_3,
  short_wheel_name_4,
  short_wheel_name_5,
  short_wheel_name_6,
  short_wheel_name_7,
  short_wheel_name_8,
  short_wheel_name_9,
  short_wheel_name_10,
  short_wheel_name_11,
  short_wheel_name_12,
  short_wheel_name_13,
  short_wheel_name_14,
  short_wheel_name_15,
  short_wheel_name_16,
  short_wheel_name_17,
  short_wheel_name_18,
  short_wheel_name_19,
  short_wheel_name_20,
  short_wheel_name_21,
  short_wheel_name_22,
  short_wheel_name_23,
  short_wheel_name_24
};

#endif