/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Arbritrary wheel pattern generator wheel definitions
 *
 * copyright 2014 David J. Andruczyk
 * 
 * Ardu-Stim software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ArduStim software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any ArduStim software.  If not, see http://www.gnu.org/licenses/
 *
 */
 #ifndef __WHEEL_DEFS_H__
 #define __WHEEL_DEFS_H__
 
 #include <avr/pgmspace.h>
 
 /* Wheel patterns! 
  *
  * Wheel patterns define the pin states and specific times. The ISR runs 
  * at a constant speed related to the requested RPM. The request RPM is 
  * scaled based on the LENGTH of each wheel's array.  The reference is 
  * the 60-2 which was the first decoder designed which has 120 "edges" 
  * (transitions" for each revolution of the wheel. Any other wheel that 
  * also has 120 edges has and RPM scaling factor of 1.0. IF a wheel has 
  * less edges needed to "describe" it, it's number of edges are divided by 120 to
  * get the scaling factor which is applied to the RPM calculation.
  * There is an enumeration (below) that lists the defined wheel types, 
  * as well as an array listing the rpm_scaling factors with regards to 
  * each pattern.
  * 
  * NOTE: There is MORE THAN ONE WAY to define a wheel pattern.  You can 
  * use more edges to get to 1 deg accuracy but the side effect is that 
  * your maximum RPM is capped because of that. Currently 60-2 can run 
  * up to about 60,000 RPM, 360and8 can only do about 10,000 RPM becasue 
  * it has 6x the number of edges...  The less edges, the faster it can go... :)
  * 
  * Using more edges allows you to do things like vary the dutycycle,  
  * i.e. a simple non-missing tooth 50% duty cycle wheel can be defined 
  * with only 2 entries if you really want, but I didn't do it that way 
  * for some of the simple ones as it made it seem somewhat confusing
  * to look at as it required you to keep the rpm_scaler factor in mind.  
  * Most/all patterns show the pulses you're receive for one revolution
  * of a REAL wheel on a real engine.
  */

  /*
   * Values to use within a wheel definition. 
   * 0 means no tooth on any wheel definition
   * 1 means crank tooth
   * 2 means cam1 tooth
   * 4 means cam2 tooth
   * 
   * combinations of numbers mean all of the related teeth are present, 
   * eg  3 means crank and cam1, 5 means crank and cam2, 6 means cam1 and cam2, 7 means crank, cam1 and cam2 
   */

  
  /* Wheel types we know about...
   * This enumerations is the INDEX into the Wheels[] array of structures
   * defined in main file. That struct contains pointers to the following:
   * wheel name in a user friendly string
   * pointer to the wheel edge array used by the ISR
   * RPM scaling factor (num_edges/120 for crank wheels)
   * Number of edges in the edge array above, needed by the ISR 
   */
 typedef enum { 
   DIZZY_FOUR_CYLINDER,  /* 2 evenly spaced teeth */
   DIZZY_SIX_CYLINDER,   /* 3 evenly spaced teeth */
   DIZZY_EIGHT_CYLINDER, /* 4 evenly spaced teeth */
   SIXTY_MINUS_TWO,      /* 60-2 crank only */
   SIXTY_MINUS_TWO_WITH_CAM, /* 60-2 with 2nd trigger on cam */
   SIXTY_MINUS_TWO_WITH_HALFMOON_CAM, /* 60-2 with "half moon" trigger on cam */
   THIRTY_SIX_MINUS_ONE, /* 36-1 crank only */
   TWENTY_FOUR_MINUS_ONE,
   FOUR_MINUS_ONE_WITH_CAM, /* 4-1 crank + cam */
   EIGHT_MINUS_ONE,       /* 8-1 crank only */
   SIX_MINUS_ONE_WITH_CAM,/* 6-1 crank + cam */
   TWELVE_MINUS_ONE_WITH_CAM, /* 12-1 crank + cam */
   FOURTY_MINUS_ONE,      /* Ford V-10 40-1 crank only */
   DIZZY_FOUR_TRIGGER_RETURN,  /* dizzy 4 cylinder signal, 40deg on 50 deg off */
   ODDFIRE_VR,            /* Oddfire V-twin */
   THIRTY_SIX_MINUS_TWO_TWO_TWO, /* 36-2-2-2 crank only H4 */
   THIRTY_SIX_MINUS_TWO_TWO_TWO_H6, /* 36-2-2-2 crank only H6 */
   THIRTY_SIX_MINUS_TWO_TWO_TWO_WITH_CAM, /* 36-2-2-2 crank and cam */
   GM_LS1_CRANK_AND_CAM,  /* GM LS1 24 tooth with cam */
   GM_FOUR_TOOTH_WITH_CAM, /* GM 4 even crank with half moon cam */
   GM_SIX_TOOTH_WITH_CAM, /* GM 4 even crank with half moon cam */
   GM_EIGHT_TOOTH_WITH_CAM, /* GM 4 even crank with half moon cam */
   TWELVE_WITH_CAM, //12 evenly spaced crank teeth and a single cam tooth
   TWENTY_FOUR_WITH_CAM, //24 evenly spaced crank teeth and a single cam tooth
   SUBARU_SIX_SEVEN,      /* Subaru 6 crank, 7 cam */
   MAX_WHEELS,
 }WheelType;

/* Name strings for EACH wheel type, for serial UI */
 const char dizzy_four_cylinder_friendly_name[] PROGMEM = "4 cylinder dizzy";
 const char dizzy_six_cylinder_friendly_name[] PROGMEM = "6 cylinder dizzy";
 const char dizzy_eight_cylinder_friendly_name[] PROGMEM = "8 cylinder dizzy";
 const char sixty_minus_two_friendly_name[] PROGMEM = "60-2 crank only";
 const char sixty_minus_two_with_cam_friendly_name[] PROGMEM = "60-2 crank and cam";
 const char sixty_minus_two_with_halfmoon_cam_friendly_name[] PROGMEM = "60-2 crank and 'half moon' cam";
 const char thirty_six_minus_one_friendly_name[] PROGMEM = "36-1 crank only";
 const char twenty_four_minus_one_friendly_name[] PROGMEM = "24-1 crank only";
 const char four_minus_one_with_cam_friendly_name[] PROGMEM = "4-1 crank wheel with cam";
 const char eight_minus_one_friendly_name[] PROGMEM = "8-1 crank only (R6)";
 const char six_minus_one_with_cam_friendly_name[] PROGMEM = "6-1 crank with cam";
 const char twelve_minus_one_with_cam_friendly_name[] PROGMEM = "12-1 crank with cam";
 const char fourty_minus_one_friendly_name[] PROGMEM = "40-1 crank only (Ford V10)";
 const char dizzy_four_trigger_return_friendly_name[] PROGMEM = "Distributor style 4 cyl 50deg off, 40 deg on";
 const char oddfire_vr_friendly_name[] PROGMEM = "odd fire 90 deg pattern 0 and 135 pulses";
 const char thirty_six_minus_two_two_two_friendly_name[] PROGMEM = "36-2-2-2 H4 Crank only";
 const char thirty_six_minus_two_two_two_h6_friendly_name[] PROGMEM = "36-2-2-2 H6 Crank only";
 const char thirty_six_minus_two_two_two_with_cam_friendly_name[] PROGMEM = "36-2-2-2 Crank and cam";
 const char gm_ls1_crank_and_cam_friendly_name[] PROGMEM = "GM LS1 crank and cam";
 const char gm_four_tooth_with_cam_friendly_name[] PROGMEM = "GM 4 even-tooth crank with 1 tooth cam";
 const char gm_six_tooth_with_cam_friendly_name[] PROGMEM = "GM 6 even-tooth crank with 1 tooth cam";
 const char gm_eight_tooth_with_cam_friendly_name[] PROGMEM = "GM 8 even-tooth crank with 1 tooth cam";
 const char twelve_with_cam_friendly_name[] PROGMEM = "12/1 (12 crank with cam)";
 const char twenty_four_with_cam_friendly_name[] PROGMEM = "24/1 (24 crank with cam)";
 const char subaru_six_seven_name_friendly_name[] PROGMEM = "Subaru 6/7 crank and cam";

 /* Very simple 50% duty cycle */
 const unsigned char dizzy_four_cylinder[] PROGMEM = 
   { /* dizzy 4 cylinder */
     1,0,1,0 /* two pulses per crank revolution (one per cylinder) */
   };
   
 /* Very simple 50% duty cycle */
 const unsigned char dizzy_six_cylinder[] PROGMEM = 
   { /* dizzy 6 cylinder */
     1,0,1,0,1,0 /* three pulses per crank revolution (one per cylinder) */
   };
   
 /* Very simple 50% duty cycle */
 const unsigned char dizzy_eight_cylinder[] PROGMEM = 
   { /* dizzy 8 cyl */
     1,0,1,0,1,0,1,0 /* four pulses per crank revolution (one per cylinder) */
   };
   
 /* Standard bosch 60-2 pattern, 50% duty cyctle during normal teeth */
 const unsigned char sixty_minus_two[] PROGMEM = 
   { /* 60-2 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 1-5 */ 
     1,0,1,0,1,0,1,0,1,0,  /* teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 31-35 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 36-40 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 41-45 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 46-50 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 51-55 */
     1,0,1,0,1,0,0,0,0,0   /* teeth 56-58 and 59-60 MISSING */
   };
 
 /* Bosch 60-2 pattern with 2nd trigger on rotation 2, 
  * 50% duty cyctle during normal teeth */
 const unsigned char sixty_minus_two_with_cam[] PROGMEM = 
   { /* 60-2 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 1-5 */ 
     1,0,1,0,1,0,1,0,1,0,  /* teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 31-35 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 36-40 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 41-45 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 46-50 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 51-55 */
     1,0,1,0,1,0,0,0,0,0,  /* teeth 56-58 and 59-60 MISSING */
     1,0,1,0,1,0,1,0,1,0,  /* Second revolution teeth 1-5 */ 
     1,0,1,0,1,0,1,0,1,0,  /* teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 31-35 */
     1,2,1,0,1,0,1,0,1,0,  /* teeth 36-40, Cam trigger on latter half of 36th */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 41-45 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 46-50 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 51-55 */
     1,0,1,0,1,0,0,0,0,0   /* teeth 56-58 and 59-60 MISSING */
   };
 
 /* 60-2 pattern with half moon cam trigger (cam input is high for one rotation and low for second rotation),
  * 50% duty cyctle during normal teeth */
 const unsigned char sixty_minus_two_with_halfmoon_cam[] PROGMEM = 
   { /* 60-2 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 1-5 */ 
     1,0,1,0,1,0,1,0,1,0,  /* teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 31-35 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 36-40 */
     1,0,1,0,1,0,3,2,3,2,  /* teeth 41-45, Cam trigger goes high on 44th tooth */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 46-50 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 51-55 */
     3,2,3,2,3,2,2,2,2,2,  /* teeth 56-58 and 59-60 MISSING */
     3,2,3,2,3,2,3,2,3,2,  /* Second revolution teeth 1-5 */ 
     3,2,3,2,3,2,3,2,3,2,  /* teeth 6-10 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 11-15 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 16-20 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 21-25 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 26-30 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 31-35 */
     3,2,3,2,3,2,3,2,3,2,  /* teeth 36-40 */
     3,2,3,2,3,2,1,0,1,0,  /* teeth 41-45, Cam trigger goes low on 43th tooth on rotation 2 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 46-50 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 51-55 */
     1,0,1,0,1,0,0,0,0,0   /* teeth 56-58 and 59-60 MISSING */
   };
 
 /* Standard ford/mazda and aftermarket 36-1 pattern, 50% duty cyctle during normal teeth */  
 const unsigned char thirty_six_minus_one[] PROGMEM = 
   { /* 36-1 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 1-5 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0,  /* teeth 31-35 */
     0,0                   /* MISSING 36th tooth  */
   }; 
   
  /* Standard ford/mazda and aftermarket 36-1 pattern, 50% duty cyctle during normal teeth */  
 const unsigned char twenty_four_minus_one[] PROGMEM = 
   { /* 36-1 */
     1,0,1,0,1,0,  /* teeth 1-3 */
     1,0,1,0,1,0,  /* teeth 4-6 */
     1,0,1,0,1,0,  /* teeth 7-9 */
     1,0,1,0,1,0,  /* teeth 10-12 */
     1,0,1,0,1,0,  /* teeth 13-15 */
     1,0,1,0,1,0,  /* teeth 16-18 */
     1,0,1,0,1,0,  /* teeth 19-21 */
     1,0,1,0,0,0,  /* teeth 22-21 */
   }; 
   
 /* 4-1 crank signal 50% duty cycle with Cam tooth enabled during the second rotation prior to tooth 2 */
 const unsigned char four_minus_one_with_cam[] PROGMEM = 
   { /* 4-1 with cam */
     0,1,0,1,0,1,0,0,  /* Teeth 1-3, then MISSING */
     0,1,2,1,0,1,0,0   /* Tooth 5, 2nd trigger on cam between 5 and 6 
						  then 6 and 7 and MISSING 8th */
   };
   
 /* Yamaha R6 crank trigger 8 teeth missing one, (22.5deg low, 22.5deg high) 50% duty cycle during normal teeth */
 const unsigned char eight_minus_one[] PROGMEM = 
   { /* 8-1 */
     0,1,0,1,0,1,0,1,  /* Teeth 1-4 */
     0,1,0,1,0,1,0,0   /* Teeth 5-7, then MISSING */
   };
  
  /* 40deg low, 20 deg high per tooth, cam signal on second rotation during 40deg low portion of 3rd tooth */
 const unsigned char six_minus_one_with_cam[] PROGMEM = 
   { /* 6-1 with cam */
     0,0,1,0,0,1,0,0,1,  /* Teeth 1-3 */
     0,0,1,0,0,1,0,0,0,  /* Teeth 4 and 5 and MISSING 6th */
     0,0,1,0,0,1,2,2,1,  /* 2nd rev teeth 7 and 8, then 2nd trigger on cam between 8 and 9 */
     0,0,1,0,0,1,0,0,0   /* teeth 10 and 11 then missing 12th */
   };
  
  /* 25 deg low, 5 deg high, #12 is missing,  cam is high for 25 deg on second crank rotation just after tooth 21 (9) */
 const unsigned char twelve_minus_one_with_cam[] PROGMEM = 
   { /* 12-1 with cam */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 1 and 2 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 3 and 4 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 5 and 6 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 7 and 8 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 9 and 10 */
	   0,0,0,0,0,1,0,0,0,0,0,0, /* Tooth 11 and MISSING twelth */
     //0,0,0,0,0,1,0,0,0,0,0,1, /* Tooth 11 and WITHOUT MISSING twelth */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* 2nd rotation: Teeth 13 and 14 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 15 and 16 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 17 and 18 */
	   0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 19 and 20 */
	   0,0,0,0,0,1,2,2,2,2,2,1, /* Tooth 21 and 22,  2nd trigger on cam between teeth 21 and 22 for 25 deg */
	   0,0,0,0,0,1,0,0,0,0,0,0  /* Totth 23 and MISSING 24th */
     //0,0,0,0,0,1,0,0,0,0,0,1  /* Totth 23 and WITHOUT MISSING 24th */
   };
   
  /* Ford V10 version of EDIS with 40 teeth instead of 36, 50% duty cycle during normal teeth.. */
 const unsigned char fourty_minus_one[] PROGMEM = 
   { /* 40-1 */
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 1-5 */
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 6-10 */ 
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 11-15 */ 
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 16-20 */ 
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 21-25 */ 
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 26-30 */ 
     0,1,0,1,0,1,0,1,0,1,  /* Teeth 31-35 */ 
     0,1,0,1,0,1,0,1,0,0   /* Teeth 36-39 and MISSING 40th tooth */ 
   };
  
  /* 50deg off, 40 deg on dissy style signal */
  const unsigned char dizzy_four_trigger_return[] PROGMEM = 
    { /* dizzy trigger return */
      0,0,0,0,0,1,1,1,1l  /* Simple off/on signal, 50deg off, 40 deg on */
    };
   
  /* Oddfire V twin  135/225 split */
  const unsigned char oddfire_vr[] PROGMEM = 
    { /* Oddfire VR */
      1,0,0,0,0,0,0,0,0,1,0,0, /* Tooth 1 and 2 at 0 deg and 135 deg, then 225 off */
      0,0,0,0,0,0,0,0,0,0,0,0 
    };
  
  const unsigned char thirty_six_minus_two_two_two[] PROGMEM = 
    {
      //H4 version
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,0,0,0,0,
      1,0,0,0,0,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,0,0,0,0,1,0,1,0,
      1,0 
    };
  
  const unsigned char thirty_six_minus_two_two_two_h6[] PROGMEM = 
    {
      //H6 version
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,0,0,
      0,0,1,0,1,0,1,0,1,0,
      1,0,1,0,1,0,1,0,1,0,
      1,0,0,0,0,0,1,0,0,0,
      0,0 
    };
  
  const unsigned char thirty_six_minus_two_two_two_with_cam[] PROGMEM = 
    { /* 36-2-2-2 H4 with cam  */
      1,0,0,2,0,0,1,0,0,0, /* Tooth one, missing teeth 2,3 and 5, 2nd trigger during teeth 2 and 3 */
      0,0,1,0,1,0,1,0,1,0, /* Missing tooth 6, then 7-10 */
      1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
      1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */ 
      1,0,1,0,0,2,0,0,1,0, /* Teeth 21-2sing 23-24, then 25, 2nd trigger during teeth 23 and 24 */ 
      1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */ 
      1,0,1,0,1,0,1,0,1,0, /* Teeth 31-35 */ 
      1,0,                  /* 36th Tooth */
      1,0,0,0,0,0,1,0,0,0, /* Tooth one, missing teeth 2,3 and 5 */
      0,0,1,0,1,2,1,0,1,0, /* Missing tooth 6, then 7-10 */
      1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
      1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */ 
      1,0,1,0,0,0,0,0,1,0, /* Teeth 21-22 missing 23-24, then 25 */ 
      1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */ 
      1,0,1,0,1,0,1,0,1,0, /* Teeth 31-35 */ 
      1,0                  /* 36th Tooth */
    };
   
  const unsigned char gm_ls1_crank_and_cam[] PROGMEM = 
    { /* GM LS1 24 tooth crank snd 1 tooth cam */
	  /* 12 deg low, 3 deg high, 3 deg low,
	   * 12 deg high, 3deg low, 12 deg high,
	   * 3 deg low, 12 deg high, 3 deg low,
	   * 12 deg high, 3 deg low, 12 deg high,
	   * 12 deg low, 3 deg high, 3 deg low,
	   * 12 deg high, 3 deg low, 12 deg high,
	   * 3 deg low, 12 deg high, 12 deg low,
	   * 3 deg high, 12 deg low, 3 deg high,
	   * 3 deg low, 12 deg high, 3 deg low,
	   * 12 deg high, 12 deg low, 3 deg high,
	   * 12 deg low, 3 deg high, 12 deg low,
	   * 3 deg high, 12 deg low, 3 deg high,
	   * 3 deg low, 12 deg high, 12 deg low,
	   * 3 deg high, 3 deg low, 12 deg high,
	   * 12 deg low, 3 deg high, 12 deg low,
	   * 3 deg high, 12 deg low, 3 deg high
	   * Second rotation is the SAME pattern
	   * with cam signal held high for 360 
	   * crank degrees */
      4,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,
      1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,
      1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
      0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,
      1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
      0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,
      1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
      0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,
      1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
      0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,
      1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
      0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
      0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
      2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,2,2,2,3,3, /* Second rotation with */
      3,3,3,3,3,3,3,3,3,3,2,2,2,3,3,3,3,3,3,3, /* Cam tooth enabled */
      3,3,3,3,3,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,
      2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,3,3,
      3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,
      2,2,3,3,3,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,
      2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,3,3,
      3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,
      2,2,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,
      2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,3,3,
      3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,
      2,2,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,
      2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,2,2,2,3,3,
      3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,
      2,2,3,3,3,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,
      2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,2,2,2,2,2,
      2,2,2,2,2,2,2,3,3,3,2,2,2,2,2,2,2,2,2,2,
      2,2,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,
    };

 /* 50% dutycle, 4 tooth + 1 cam */
 const unsigned char gm_four_tooth_with_cam[] PROGMEM = 
   { /* 4 cylinder with 1 cam pulse for 360 crank degrees */
     1,0,1,0,3,2,3,2 /* two pulses per crank revolution (one per cylinder) */
   };
   
 /* 50% dutycle, 6 tooth + 1 cam */
 const unsigned char gm_six_tooth_with_cam[] PROGMEM = 
   { /* 6 cylinder with 1 cam pulse for 360 crank degrees */
     1,0,1,0,1,0,3,2,3,2,3,2 /* three pulses per crank revolution (one per cylinder) */
   };
   
 /*  50% dutycle, 8 tooth + 1 cam */
 const unsigned char gm_eight_tooth_with_cam[] PROGMEM = 
   { /* 8 cylinder with 1 cam pulse for 360 crank degrees  */
     1,0,1,0,1,0,1,0,3,2,3,2,3,2,3,2 /* four pulses per crank revolution (one per cylinder) */
   };
   
 /* 25 deg low, 5 deg high, #12 is missing,  cam is high for 25 deg on second crank rotation just after tooth 21 (9) */
 const unsigned char twelve_with_cam[] PROGMEM = 
   { /* 12-1 with cam */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 1 and 2 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 3 and 4 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 5 and 6 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 7 and 8 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 9 and 10 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Tooth 11 and 12 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* 2nd rotation: Teeth 13 and 14 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 15 and 16 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 17 and 18 */
     0,0,0,0,0,1,0,0,0,0,0,1, /* Teeth 19 and 20 */
     0,0,0,0,0,1,2,2,2,2,2,1, /* Tooth 21 and 22,  2nd trigger on cam between teeth 21 and 22 for 25 deg */
     0,0,0,0,0,1,0,0,0,0,0,1  /* Totth 23 and 24th */
   };

 /* 25 deg low, 5 deg high, #12 is missing,  cam is high for 25 deg on second crank rotation just after tooth 21 (9) */
 const unsigned char twenty_four_with_cam[] PROGMEM = 
   { /* 24/1 with cam */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 1 and 2 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 3 and 4 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 5 and 6 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 7 and 8 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 9 and 10 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Tooth 11 and 12 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* 2nd rotation: Teeth 13 and 14 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 15 and 16 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 17 and 18 */
     0,0,1,0,0,1,0,0,1,0,0,1, /* Teeth 19 and 20 */
     0,0,1,0,0,1,2,2,3,2,2,1, /* Tooth 21 and 22,  2nd trigger on cam between teeth 21 and 22 for 25 deg */
     0,0,1,0,0,1,0,0,1,0,0,1  /* Totth 23 and 24th */
   };

  const unsigned char subaru_six_seven[] PROGMEM =
   { /* 6/7 */
    /* Cyl 1 TDC */
      0,0,0,0,0,2,2,2,0,0, 0,2,2,2,0,0,0,2,2,2, /* 00-19 degrees - cam 1-2-3: 5* ATDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 20-39 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 40-59 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 60-20 degrees */
      0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 80-99 degrees - crank 1: 83* ATDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0,0, /* 100 degrees - crank 2: 115* ATDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 120 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 140 degrees */
      0,0,0,0,0,0,0,0,0,0, 1,1,1,0,0,0,0,0,0,0, /* 160 degrees - crank 3: 170* ATDC */

    /* Cyl 2 TDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 180 degrees */
      2,2,2,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 200 degrees - cam 4: 20* ATDC (200*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 220 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 240 degrees */
      0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 260 degrees - crank 4: 83* ATDC (263*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0,0, /* 280 degrees - crank 5: 115* ATDC (295*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 300 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 320 degrees */
      0,0,0,0,0,0,0,0,0,0, 1,1,1,0,0,0,0,0,0,0, /* 340 degrees - crank 6: 170* ATDC (350*) */

    /* Cyl 3 TDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 360 degrees */
      2,2,2,0,0,0,2,2,2,0, 0,0,0,0,0,0,0,0,0,0, /* 380 degrees - cam 5-6: 20* ATDC (380*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 400 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 420 degrees */
      0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 440 degrees - crank 1: 83* ATDC (443*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0,0, /* 460 degrees - crank 2: 115* ATDC (475*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 480 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 500 degrees */
      0,0,0,0,0,0,0,0,0,0, 1,1,1,0,0,0,0,0,0,0, /* 520 degrees - crank 3: 170* ATDC */

    /* Cyl 4 TDC */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 540 degrees */
      2,2,2,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 560 degrees - cam 7: 20* ATDC (560*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 580 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 600 degrees */
      0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 620 degrees - crank 4: 83* ATDC (623*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0,0, /* 640 degrees - crank 5: 115* ATDC (655*) */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 660 degrees */
      0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, /* 680 degrees */
      0,0,0,0,0,0,0,0,0,0, 1,1,1,0,0,0,0,0,0,0  /* 700 degrees - crank 6: 170* ATDC (710*) */
    };

  #endif