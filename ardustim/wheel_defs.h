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
   OPTISPARK_LT1,         /* Optispark 360 and 8 */
   TWELVE_MINUS_THREE,    /* 12-3 */
   THIRTY_SIX_MINUS_TWO_TWO_TWO, /* 36-2-2-2 crank only H4 */
   THIRTY_SIX_MINUS_TWO_TWO_TWO_H6, /* 36-2-2-2 crank only H6 */
   THIRTY_SIX_MINUS_TWO_TWO_TWO_WITH_CAM, /* 36-2-2-2 crank and cam */
   FOURTY_TWO_HUNDRED_WHEEL, /* 4200 wheel */
   THIRTY_SIX_MINUS_ONE_WITH_CAM_FE3, /* Mazda F3 36-1 crank and cam */
   SIX_G_SEVENTY_TWO_WITH_CAM, /* Mitsubishi DOHC CAS and TCDS 6G72 */
   BUELL_ODDFIRE_CAM,     /* Buell 45 deg cam wheel */
   GM_LS1_CRANK_AND_CAM,  /* GM LS1 24 tooth with cam */
   GM_58x_LS_CRANK_4X_CAM, /* GM 58x LS crank 4x cam wheel */
   LOTUS_THIRTY_SIX_MINUS_ONE_ONE_ONE_ONE, /* Lotus crank wheel 36-1-1-1-1 */
   HONDA_RC51_WITH_CAM,   /* Honda oddfire 90 deg V-twin */
   THIRTY_SIX_MINUS_ONE_WITH_SECOND_TRIGGER, /* From jimstim */
   WEBER_IAW_WITH_CAM, /* From jimstim IAW weber-marelli */
   FIAT_ONE_POINT_EIGHT_SIXTEEN_VALVE_WITH_CAM, /* Fiat 1.8 16V from jimstim */
   THREE_SIXTY_NISSAN_CAS, /*from jimstim 360 tooth cas with 6 slots */
   TWENTY_FOUR_MINUS_TWO_WITH_SECOND_TRIGGER, /* Mazda CAS 24-1 inner ring single pulse outer ring */
   YAMAHA_EIGHT_TOOTH_WITH_CAM, /* 02-03 Yamaha R1, seank */
   MITSUBISH_4g63_4_2,
   AUDI_135_WITH_CAM,
   HONDA_D17_NO_CAM,
   MAZDA_323_AU,
   DAIHATSU_3CYL,
   MIATA_9905,
   TWELVE_WITH_CAM, //12 evenly spaced crank teeth and a single cam tooth
   TWENTY_FOUR_WITH_CAM, //24 evenly spaced crank teeth and a single cam tooth
   SUBARU_SIX_SEVEN,      /* Subaru 6 crank, 7 cam */
   GM_7X,                 /* GM 7X pattern. 6 even teeth with 1 extra uneven tooth */
   FOUR_TWENTY_A,         /* DSM 420a */
   FORD_ST170,            /* Ford ST170 */
   MITSUBISHI_3A92,        /* Mitsubishi 3cylinder 3A92 */
   TOYOTA_4AGE_CAS,           /*Toyota 4AGE CAS, 4 teeth and one cam tooth*/
   TOYOTA_4AGZE,           /*Toyota 4AGZE, 24 teeth and one cam tooth*/
   SUZUKI_DRZ400,         /* Suzuki DRZ-400 6 coil "tooths", 2 uneven crank tooths */
   JEEP2000_4CYL,  /* Jeep 2.5 4cyl aka jeep2000_4cyl */
   JEEP2000_6CYL,  /* Jeep 4.0 6cyl aka jeep2000_6cyl */
   VIPER_96_02, // Dodge Viper 1996-2002 wheel pattern
   THIRTY_SIX_MINUS_TWO_WITH_ONE_CAM, // 36-2 with  1 tooth cam - 2jz-gte VVTI crank pulley + non-vvti cam
   GM_40_OSS, // GM 40 tooth wheel no skips for transmission OSS simulation

   MAX_WHEELS,
 }WheelType;

/* Name strings for EACH wheel type, optimized for 20x4 LCD display */
 const char dizzy_four_cylinder_friendly_name[] PROGMEM = "4 Cyl Dizzy";
 const char dizzy_six_cylinder_friendly_name[] PROGMEM = "6 Cyl Dizzy";
 const char dizzy_eight_cylinder_friendly_name[] PROGMEM = "8 Cyl Dizzy";
 const char sixty_minus_two_friendly_name[] PROGMEM = "60-2 Crank";
 const char sixty_minus_two_with_cam_friendly_name[] PROGMEM = "60-2 Crank+Cam";
 const char sixty_minus_two_with_halfmoon_cam_friendly_name[] PROGMEM = "60-2 Half Moon Cam";
 const char thirty_six_minus_one_friendly_name[] PROGMEM = "36-1 Crank";
 const char twenty_four_minus_one_friendly_name[] PROGMEM = "24-1 Crank";
 const char four_minus_one_with_cam_friendly_name[] PROGMEM = "4-1 Crank+Cam";
 const char eight_minus_one_friendly_name[] PROGMEM = "8-1 Crank (R6)";
 const char six_minus_one_with_cam_friendly_name[] PROGMEM = "6-1 Crank+Cam";
 const char twelve_minus_one_with_cam_friendly_name[] PROGMEM = "12-1 Crank+Cam";
 const char fourty_minus_one_friendly_name[] PROGMEM = "40-1 Ford V10";
 const char dizzy_four_trigger_return_friendly_name[] PROGMEM = "4 Cyl Dizzy Return";
 const char oddfire_vr_friendly_name[] PROGMEM = "Oddfire VR 90deg";
 const char optispark_lt1_friendly_name[] PROGMEM = "GM OptiSpark LT1";
 const char twelve_minus_three_friendly_name[] PROGMEM = "12-3 Oddball";
 const char thirty_six_minus_two_two_two_friendly_name[] PROGMEM = "36-2-2-2 H4";
 const char thirty_six_minus_two_two_two_h6_friendly_name[] PROGMEM = "36-2-2-2 H6";
 const char thirty_six_minus_two_two_two_with_cam_friendly_name[] PROGMEM = "36-2-2-2 +Cam";
 const char fourty_two_hundred_wheel_friendly_name[] PROGMEM = "GM 4200 Wheel";
 const char thirty_six_minus_one_with_cam_fe3_friendly_name[] PROGMEM = "Mazda FE3 36-1+Cam";
 const char six_g_seventy_two_with_cam_friendly_name[] PROGMEM = "Mitsubishi 6G72";
 const char buell_oddfire_cam_friendly_name[] PROGMEM = "Buell Oddfire Cam";
 const char gm_ls1_crank_and_cam_friendly_name[] PROGMEM = "GM LS1 Crank+Cam";
 const char gm_ls_58X_crank_and_4x_cam_friendly_name[] PROGMEM = "GM 58x+4x Cam";
 const char lotus_thirty_six_minus_one_one_one_one_friendly_name[] PROGMEM = "Lotus 36-1-1-1-1";
 const char honda_rc51_with_cam_friendly_name[] PROGMEM = "Honda RC51+Cam";
 const char thirty_six_minus_one_with_second_trigger_friendly_name[] PROGMEM = "36-1 2nd Trigger";
 const char weber_iaw_with_cam_friendly_name[] PROGMEM = "Weber IAW 8+2";
 const char fiat_one_point_eight_sixteen_valve_with_cam_friendly_name[] PROGMEM = "Fiat 1.8 16V";
 const char three_sixty_nissan_cas_friendly_name[] PROGMEM = "Nissan 360 CAS";
 const char twenty_four_minus_two_with_second_trigger_friendly_name[] PROGMEM = "Mazda CAS 24-2";
 const char yamaha_eight_tooth_with_cam_friendly_name[] PROGMEM = "Yamaha R1 8T+Cam";
 const char mitsubishi_4g63_4_2_friendly_name[] PROGMEM = "Mitsubishi 4G63";
 const char audi_135_with_cam_friendly_name[] PROGMEM = "Audi 135T+Cam";
 const char honda_d17_no_cam_friendly_name[] PROGMEM = "Honda D17 12+1";
 const char mazda_323_au_friendly_name[] PROGMEM = "Mazda 323 AU";
 const char daihatsu_3cyl_friendly_name[] PROGMEM = "Daihatsu 3+1";
 const char miata_9905_friendly_name[] PROGMEM = "Miata 99-05";
 const char twelve_with_cam_friendly_name[] PROGMEM = "12/1 Crank+Cam";
 const char twenty_four_with_cam_friendly_name[] PROGMEM = "24/1 Crank+Cam";
 const char subaru_six_seven_name_friendly_name[] PROGMEM = "Subaru 6/7";
 const char gm_seven_x_friendly_name[] PROGMEM = "GM 7X";
 const char four_twenty_a_friendly_name[] PROGMEM = "DSM 420a";
 const char ford_st170_friendly_name[] PROGMEM = "Ford ST170";
 const char mitsubishi_3A92_friendly_name[] PROGMEM = "Mitsubishi 3A92";
 const char Toyota_4AGE_CAS_friendly_name[] PROGMEM = "Toyota 4AGE";
 const char Toyota_4AGZE_friendly_name[] PROGMEM = "Toyota 4AGZE";
 const char Suzuki_DRZ400_friendly_name[] PROGMEM = "Suzuki DRZ400";
 const char Jeep_2000_4cyl_friendly_name[] PROGMEM = "Jeep 2000 4Cyl";
 const char VIPER9602_friendly_name[] PROGMEM = "Viper V10 96-02";
 const char thirty_six_minus_two_with_second_trigger_friendly_name[] PROGMEM = "36-2+1T Cam";
 const char GM_40_Tooth_Trans_OSS_friendly_name[] PROGMEM = "GM 40T Trans OSS";

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
  
  /* GM LT1 360 and 8 wheel, see http://powerefi.com/files/opti-LT1-count.JPG */
  const unsigned char optispark_lt1[] PROGMEM = 
    { /* Optispark 360 outside teeth, 8 varying inside teeth */
    /* 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 0-30 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 31-60 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3, /* 61-90 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 91-120 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3,
      2,3,2,3,2,3,2,3,2,3,0,1,0,1,0,1,0,1,0,1, /* 121-150 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3, /* 151-180 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 181-210 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, 
      0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3,
      2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 211-240 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3, /* 241-270 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 271-300 */
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3,
      2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 301-330 */
      2,3,2,3,2,3,2,3,2,3,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,2,3  /* 331-360 */
  };
  
  const unsigned char twelve_minus_three[] PROGMEM = 
    { /* 12-3, http://www.msextra.com/doc/triggers/12_3_wheel_133.jpg */
      1,0,0,0,1,0,0,0,  /* Teeth 1-2 */
      1,0,0,0,1,0,0,0,  /* Teeth 3-4 */
      1,0,0,0,1,0,0,0,  /* Teeth 5-6 */
      1,0,0,0,1,0,0,0,  /* Teeth 7-8 */
      1,0,0,0,0,0,0,0,  /* Tooth 9 and MISSING 10th */
      0,0,0,0,0,0,0,0   /* MISSING Teeth 11-12 */
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
   
  
  const unsigned char fourty_two_hundred_wheel[] PROGMEM = 
    { /* 4200 wheel http://msextra.com/doc/triggers/4200_timing.pdf */
		/* 55 deg high, 5 deg low, 55 deg high, 5 deg low,
		 * 5 deg high, 5 deg low, 45 deg high, 5 deg low,
		 * 55 deg high, 5 deg low, 65 deg high, 5 deg low,
		 * 45 deg high, 5 deg low, (360 degreees ) */
      1,1,1,1,1,1,1,1,1,1, 
	  1,0,1,1,1,1,1,1,1,1, 
	  1,1,1,0,1,0,1,1,1,1, 
	  1,1,1,1,1,0,1,1,1,1,
	  1,1,1,1,1,1,1,0,1,1,
	  1,1,1,1,1,1,1,1,1,1,
	  1,0,1,1,1,1,1,1,1,1,
	  1,0
  };

 /* Mazda F3 36-1 with cam */
 const unsigned char thirty_six_minus_one_with_cam_fe3[] PROGMEM = 
   { /* 36-1 with cam, 3 cam teeth, 2 180deg from each other */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 0-55 deg */
     1,0,1,0,1,0,3,2,3,0,1,0, /* 60-115 deg  cam tooth at 90 deg crank for 15 crank degrees */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 120-175 deg */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 180-235 deg */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 240-295 deg */
     1,0,1,0,1,0,1,0,1,0,0,0, /* 300-355 deg Crank missing tooth at end */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 360-415 deg */
     3,2,3,0,1,0,3,2,3,0,1,0, /* 420-475 deg 2 more cam teeth at 420 and 450 deg, 15 crank degrees each */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 480-535 deg */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 540-595 deg */
     1,0,1,0,1,0,1,0,1,0,1,0, /* 600-655 deg */
     1,0,1,0,1,0,1,0,1,0,0,0  /* 660-715 deg Crank missing tooth at end */
   }; 
  
  /* Mitsubishi 6g72 crank/cam */
  const unsigned char six_g_seventy_two_with_cam[] PROGMEM = 
    { /* Mitsubishi 6g72 */
	  /* Crank signal's are 50 deg wide, and one per cylinder
	   * Cam signals have 3 40 deg wide teeh and one 85 deg wide tooth
	   * Counting both From TDC#1
	   * Crank: 40 deg high, 70 deg low (repeats whole cycle)
	   * Cam: 70 deg high, 80 deg low, 40 deg high, 150 deg low,
	   * 40 deg high, 130 deg low, 40 deg high, 155 deg low 
	   */
      3,3,3,3,3,3,3,3,3,2,
      2,2,2,2,0,0,0,0,0,0,
      0,0,0,1,1,1,1,1,1,1,
      3,3,3,2,2,2,2,2,0,0,
      0,0,0,0,0,0,0,1,1,1,
      1,1,1,1,1,1,1,0,0,0,
      0,0,0,0,0,0,0,0,2,2,
      2,3,3,3,3,3,1,1,1,1,
      1,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,1,1,1,1,1,
      1,1,3,3,3,2,2,2,2,2,
      0,0,0,0,0,0,0,0,0,1,
      1,1,1,1,1,1,1,1,1,0,
      0,0,0,0,0,0,0,0,0,0,
      0,2,2,3
    };
   
  const unsigned char buell_oddfire_cam[] PROGMEM = 
    { /* Buell oddfire cam wheel */
	  /* Wheel is a cam wheel (degress are in crank degrees 
	   * 36 deg high, 54 deg low,
	   * 36 deg high, 54 deg low,
	   * (Rear at TDC) 36 deg high,
	   * 1889 deg low, 36 deg high
	   * 54 deg low, 36 deg high,
	   * 54 deg low, (Front at TDC),
	   * 36 deg high, 99 deg low
	   */
	  1,1,1,1,0,0,0,0,0,0, /* Tooth 1 (36 deg) and 54 deg space */
	  1,1,1,1,0,0,0,0,0,0, /* Tooth 2 (36 deg) and 54 deg space */
	  1,1,1,1,0,0,0,0,0,0, /* Tooth 3 (36 deg) and begin of 189 deg space */
	  0,0,0,0,0,0,0,0,0,0, /* Part of 189 deg space */
	  0,0,0,0,0,1,1,1,1,0, /* Tail of 189 deg space, 36 deg tooth, begin of 54 deg space */
	  0,0,0,0,0,1,1,1,1,0, /* Tail of 54 deg space, 36 deg tooth, begin of 54 deg space */
	  0,0,0,0,0,1,1,1,1,0, /* Tail of 54 deg space, last 36 deg tooth, begin of 99 deg space */
	  0,0,0,0,0,0,0,0,0,0  /* Tail of 99 deg space */
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

 //Added by Dale Follett of Twisted Builds LLC for GM gen4 LS 58x 4x crank cam simulation.
 const unsigned char GM_LS_58X_crank_and_4x_cam[] PROGMEM = 
    { //58x LS crank 4x LS cam
      1,0,1,0,3,2,3,2,3,2, //1-5
      3,2,3,2,3,2,1,0,1,0, //6-10
      1,0,1,0,1,0,1,0,1,0, //11-15
      1,0,1,0,1,0,1,0,1,0, //16-20
      1,0,1,0,1,0,1,0,1,0, //21-25
      1,0,1,0,1,0,1,0,1,0, //26-30
      1,0,1,0,3,2,3,2,3,2, //31-35
      3,2,3,2,3,2,1,0,1,0, //36-40
      1,0,1,0,3,2,3,2,3,2, //41-45
      3,2,3,2,3,2,3,2,3,2, //46-50
      3,2,3,2,3,2,3,2,3,2, //51-55
      3,2,3,2,3,2,2,2,2,2, //56-60 - First crank revolution, last two crank teeth missing
      3,2,3,2,3,2,3,2,3,2, //61-65
      3,2,3,2,3,2,1,0,1,0, //66-70
      1,0,1,0,3,2,3,2,3,2, //71-75
      3,2,3,2,3,2,3,2,3,2, //76-80
      3,2,3,2,3,2,3,2,3,2, //81-85
      3,2,3,2,3,2,3,2,3,2, //86-90
      3,2,3,2,3,2,3,2,3,2, //91-95
      3,2,3,2,3,2,1,0,1,0, //96-100
      1,0,1,0,1,0,1,0,1,0, //101-105
      1,0,1,0,1,0,1,0,1,0, //106-110
      1,0,1,0,1,0,1,0,1,0, //111-115
      1,0,1,0,1,0,0,0,0,0, //116-120
    };
  
 /* Lotus 36-1-1-1-1 wheel, missing teeth at
  * 14, 17, 32 and 36
  */
 const unsigned char lotus_thirty_six_minus_one_one_one_one[] PROGMEM = 
   { /* 36-1-1-1-1 */
     1,0,1,0,1,0,1,0,1,0, /* teeth 1-5 */
     1,0,1,0,1,0,1,0,1,0, /* teeth 6-10 */
     1,0,1,0,1,0,0,0,1,0, /* teeth 11-13, MISSING 14, 15 */
     1,0,0,0,1,0,1,0,1,0, /* teeth 16, MISSING 17, 18-20 */
     1,0,1,0,1,0,1,0,1,0, /* teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0, /* teeth 26-30 */
     1,0,0,0,1,0,1,0,1,0, /* teeth 31, MISSING 32, 33-35 */
     0,0                  /* MISSING 36th tooth */
   }; 
 const unsigned char honda_rc51_with_cam[] PROGMEM = 
   { /* Honda RC51 oddfire 90deg Vtwin with cam */
      0,1,0,1,0,1,0,1,0,1, /* teeth 1-5 */
      0,3,0,1,0,1,0,1,0,1, /* teeth 6-10, cam triggers on tooth 6 */
      0,1,0,1,             /* Teeth 11-12 */
      0,1,0,1,0,1,0,3,0,1, /* 2nd rotation, teeth 1-5 (13-17), cam trigger on tooth 4(16)*/
      0,3,0,1,0,1,0,1,0,1, /* teeth 6-10 (18-22), cam trigger on tooth 18 */
      0,1,0,1              /* teeth 11-12, (23-24) */
   };

 /* 36-1 with second trigger pulse across teeth 33-34 on first rotation */
 const unsigned char thirty_six_minus_one_with_second_trigger[] PROGMEM = 
   { /* 36-1 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 1-5 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 6-10  */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */
     1,0,1,0,3,2,3,2,1,0, /* Teeth 31-35, second trigger during 33-34 */
     0,0,                 /* 36th tooth MISSING */
     1,0,1,0,1,0,1,0,1,0, /* Revolution 2  Teeth 1-5 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 31-35 */
     0,0                  /* 36th MISSING tooth */
   }; 
   
 const unsigned char weber_iaw_with_cam[] PROGMEM =
   { /*Weber marelli (Cosworth/Lancia) from jimstim
	   80 deg low, 10 deg high, Tooth 1
	   20 deg low, 45 deg cam pulse, 15 deg low, 10 deg high, Cam tooth 1 and crank tooth 2
	   80 deg low, 10 deg high, Crank tooth 3
	   20 deg low, 45 deg cam pulse, 15 deg low, 10 deg high, Cam tooth2 and crank tooth 4
	   80 deg low, 10 deg high, Crank tooth 5
	   80 deg low, 10 deg high  Crank tooth 6
	   80 deg low, 10 deg high, Crank tooth 7
	   80 deg low, 10 deg high  Crank tooth 8 */
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1,1, /* Teeth 1 and 2 & cam1 */
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1,1, /* Teeth 3 and 4 & cam2 */
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1, /* Teeth 5 and 6 */
	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1  /* Teeth 7 and 8 */
   };

 const unsigned char fiat_one_point_eight_sixteen_valve_with_cam[] PROGMEM =
   {
     /* Starting from TDC #1 
      * Cam is high for 40 deg, low for 20, high 170deg, low for 170, high for 20, 
      * low for 170, then high for 130
      * Crank is low for 97, high for 5, low for 27, high for 5, low for 50, high for 5,
      * low for 102, high for 5, low for 27, high for 5, low for 50, high for 5,
      * low for 102, high for 5, low for 27, high for 5, low for 50, high for 5,
      * low for 102, high for 5, low for 27, high for 5, low for 50, high for 5, low for 5
      * http://msextra.com/doc/triggers/fiat1.8-16v.jpg
      */
      /* Crankshaft degrees
     1   3   5   7   9  11  13  15  17  19  21  23  25  27  29  31  33  35  37  39  */
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
     2,2,2,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
     2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2
   };

 const unsigned char three_sixty_nissan_cas[] PROGMEM = 
 /* This version has the 360 teeth on the cam
   {
     1,2,0,2,0,2,0,2,0,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 1-40 deg
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 41-80 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 81-120 deg 
     1,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 121-160 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 161-200 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 201-240 deg 
     1,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 241-280 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 281-320 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 321-360 deg 
     1,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,3,1,3,1,3,1,3, // 361-400 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 401-340 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 441-480 deg 
     1,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2, // 481-520 deg 
     0,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 521-560 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 561-600 deg 
     1,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2, // 601-640 deg 
     0,2,0,2,0,2,0,2,0,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 641-680 deg 
     1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3, // 681-720 deg 
   };*/
   {
     /* Home teeth every 120 degrees in increasing widths (8,16,24,32,40,48) */
     /* Crankshaft degrees
     1   3   5   7   9  11  13  15  17  19  21  23  25  27  29  31  33  35  37  39  */
     2,1,0,1,0,1,0,1,0,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 1-40 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 41-80 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 81-120 deg */
     2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 121-160 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 161-200 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 201-240 deg */
     2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 241-280 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 281-320 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 321-360 deg */
     2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,2,3,2,3,2,3, /* 361-400 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 401-340 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 441-480 deg */
     2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 481-520 deg */
     0,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 521-560 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 561-600 deg */
     2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1, /* 601-640 deg */
     0,1,0,1,0,1,0,1,0,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 641-680 deg */
     2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3, /* 681-720 deg */
   };

 const unsigned char twenty_four_minus_two_with_second_trigger[] PROGMEM = 
   {
	 /* See http://postimg.org/image/pcwkrxktx/, 24-2 inner ring, single outer pulse */
	 1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,3,2,2,2,2,2, /* 11 teeth then outer and missing */
	 3,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0, /* 11 more teeth then missing */
   };

 /* eight tooth with 1 tooth cam */
 const unsigned char yamaha_eight_tooth_with_cam[] PROGMEM = 
   { /* Yamaha R1 (02-03) 8 tooth crank with 1 tooth cam */
     0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1, /* Teeth 1-4, 11.25 deg per step */
     0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1, /* teeth 5-8 */
     0,2,2,3,2,0,0,1,0,0,0,1,0,0,0,1, /* Cam tooth on 9 */
     0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1  /* Teeth 13-16 */
   };

 /* Mitsubish 4g63 aka 4/2 crank and cam */
 const unsigned char mitsubishi_4g63_4_2[] PROGMEM = 
   { //Split into 5 degree blocks (12 per line)
      2,2,2,2,2,2,2,2,2,2,2,0,
      0,0,0,0,0,0,0,0,0,1,1,1, //Start edge 6
      1,1,1,1,1,1,1,1,1,1,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,2,2,2,3,3,3,
      3,3,3,3,3,3,3,3,1,1,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,2,3,3,3,
      3,3,3,3,3,3,3,3,3,3,3,2
   };
   
 /* Mitsubish 4g63 aka 4/2 crank and cam */
 const unsigned char audi_135_with_cam[] PROGMEM = 
   { //0 - 180 degrees
     3,3,2,2,3,3,2,2,3,3,2,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,

     //181 - 360 degrees
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     
     //360 - 480
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,

     //481-720
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
     1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
     0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,2,
     
   }; 

  /* Honda D17 12+1. 5 degree per entry*/
 const unsigned char honda_d17_no_cam[] PROGMEM = 
   { //0 - 360 degrees
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,1,0,0,0,
     
     //360 - 720
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,1,0,1,0,0,0
     
   }; 

    /*
     * http://imgur.com/a/ynLWp
     */
   const unsigned char mazda_323_au[] PROGMEM = 
   {
     0,0,0,0,0,2,0,0,1,0,0,0,
     0,0,1,0,0,0,0,0,2,0,2,1, 
     0,0,0,0,0,1
   };

   /*
     * http://www.msextra.com/doc/triggers/daihatsu-trigs.txt
     * http://www.msextra.com/doc/triggers/daihatsu3cyla.jpg
     * http://jbperf.com/JimStim/wheels_default.jsw
     * 5 degree per entry
     * 
     */
   const unsigned char daihatsu_3cyl[] PROGMEM = 
   { //0 - 360 degrees
     1,1,0,0,0,0,1,1,0,0,0,0, //0-60. Primary pulse plus the additional 1 at 15 crank degrees / 30 cam degrees
     0,0,0,0,0,0,0,0,0,0,0,0, //60-120
     0,0,0,0,0,0,0,0,0,0,0,0, //120-180
     0,0,0,0,0,0,0,0,0,0,0,0, //180-240
     1,1,0,0,0,0,0,0,0,0,0,0, //240-300
     0,0,0,0,0,0,0,0,0,0,0,0, //300-360
     
     //360 - 720
     0,0,0,0,0,0,0,0,0,0,0,0, //360-420
     0,0,0,0,0,0,0,0,0,0,0,0, //420-480
     1,1,0,0,0,0,0,0,0,0,0,0, //480-540
     0,0,0,0,0,0,0,0,0,0,0,0, //540-600
     0,0,0,0,0,0,0,0,0,0,0,0, //600-660
     0,0,0,0,0,0,0,0,0,0,0,0  //660-720
   };

    /* Mitsubish 4g63 aka 4/2 crank and cam */
 const unsigned char miata_9905[] PROGMEM = 
   { //Split into 5 degree blocks (12 per line)
      0,0,0,0,0,0,2,2,0,0,0,0, //Single cam tooth
      0,0,0,0,0,0,0,1,1,0,0,0, //Pulse at 100*
      0,0,0,0,0,0,0,0,0,1,1,0, //Pulse at 170*
      0,0,0,0,0,0,0,0,0,0,0,0, //No pulse
      0,0,0,0,0,0,0,1,1,0,0,0, //Pulse at 280*
      0,0,0,0,0,0,0,0,0,1,1,0, //Pulse at 350*
      0,0,2,2,0,0,2,2,0,0,0,0, //Double cam pulses
      0,0,0,0,0,0,0,1,1,0,0,0, //Pulse at 460*
      0,0,0,0,0,0,0,0,0,1,1,0, //Pulse at 530*
      0,0,0,0,0,0,0,0,0,0,0,0, //No pulse
      0,0,0,0,0,0,0,1,1,0,0,0, //Pulse at 640
      0,0,0,0,0,0,0,0,0,1,1,0  //Pulse at 710
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
    
 /* GM 7X for 6 cylinder engines */  
 /* https://speeduino.com/forum/download/file.php?id=4743 */
 const unsigned char gm_seven_x[] PROGMEM = 
   { /* Every number represents 2 degrees */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 0-20 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 20-40 */
     0,1,1,0,0,0,0,0,0,0,  /* Degrees 40-60. Tooth #1 at 42* for 4* duration. These are actually 5* duration, but close enough */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 60-80 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 80-100 */
     0,1,1,0,0,0,1,1,0,0,  /* Degrees 100-120. Tooth #2 at 102* for 4* duration. "Extra" tooth at 112* */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 120-140 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 140-160 */
     0,1,1,0,0,0,0,0,0,0,  /* Degrees 160-180. Tooth #3 at 162* for 4* duration */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 180-200 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 200-220 */
     0,1,1,0,0,0,0,0,0,0,  /* Degrees 220-240. Tooth #4 at 222* for 4* duration */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 240-260 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 260-280 */
     0,1,1,0,0,0,0,0,0,0,  /* Degrees 280-300. Tooth #5 at 282* for 4* duration */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 300-320 */
     0,0,0,0,0,0,0,0,0,0,  /* Degrees 320-340 */
     0,1,1,0,0,0,0,0,0,0  /* Degrees 340-360. Tooth #6 at 342* for 4* duration */
   }; 

  /* DSM 420a Eclipse */
  /* https://github.com/noisymime/speeduino/issues/133 */
  const unsigned char four_twenty_a[] PROGMEM = 
   { /* Every number represents 5 degrees */
      0,0,0,0,0,0,0,0,0,0,0,2,
      2,2,2,2,2,2,2,2,2,3,3,2,
      2,3,3,2,2,3,3,2,2,3,3,2,
      2,2,2,2,2,0,0,0,0,0,0,1,
      1,1,1,1,1,1,1,1,1,1,1,0,
      0,1,1,0,0,1,1,0,0,1,1,0,
      0,0,0,0,0,0,0,0,0,0,0,2,
      2,2,2,2,2,2,2,2,2,3,3,2,
      2,3,3,2,2,3,3,2,2,3,3,2,
      2,2,2,2,2,2,2,2,2,2,2,3,
      3,3,3,3,3,1,1,1,1,1,1,0,
      0,1,1,0,0,1,1,0,0,1,1,0
   }; 

   const unsigned char ford_st170[] PROGMEM = 
   {
      0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
      1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,3,3,3,3,3,2,2,2,2,2,3,
      3,3,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,3,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,
      1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,
      1,1,1,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,1,1,0,0,0,0,0,1,
      1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,
      1,1,1,1,0,0,0,0,0,1,1,1,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,
      3,3,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
      1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
      1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
      1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,3,3,3,3,3,2,2,2,2,2,
      3,3,3,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,3,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
      1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,
      0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,
      0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,
      0,1,1,1,1,1,0,0,0,0,0,1,3,3,3,3,2,2,2,2,2,3,3,3,3,3,2,2,2,2,2,3,3,3,3,3,3,2,2,2,
      2,3,3,3,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,
      0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0
   };

  const unsigned char mitsubishi_3A92 [] PROGMEM =
  {
    1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    2,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,0,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,2,0,
    2,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    1,0,0,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,2,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    0,0,0,0
  };

   /* 4AGE CAS inside dizzy, 4 pulses 2 per crank revolution one cam pulse at 5 Deg  */

	 const unsigned char toyota_4AGE_CAS[] PROGMEM = 
	  {
      1,1,2,2,0,0,0,0,0,0,0,0, /*5 deg per */
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,
	};
  /* 4AGZE inside dizzy, 24 pulses 12 per crank revolution one cam pulse at 5 Deg  */

	 const unsigned char toyota_4AGZE[] PROGMEM = 
   { 1,1,2,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0,
     1,1,0,0,0,0,0,0,0,0,0,0
  };

  const unsigned char suzuki_DRZ400[] PROGMEM ={
      1,1,1,1,1,1,2,2,2,2,0,0,
      3,3,3,3,3,3,2,2,0,0,0,0,
      1,1,1,1,1,1,0,0,0,0,0,0,
      1,1,1,1,1,1,0,0,0,0,0,0,
      1,1,1,1,1,1,0,0,0,0,0,0,
      1,1,1,1,1,1,0,0,0,0,0,0
  };

  const unsigned char jeep_2000_4cyl[] PROGMEM = 
    { /* Every number represents 2 degrees. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 0-20. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 20-40. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 40-60. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 60-80. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 80-100.  */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 100-120 Tooth #1 at 114* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 120-140 Tooth #2 at 134* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 140-160 Tooth #3 at 154* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 160-180 Tooth #4 at 174* for 2* duration. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 180-200. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 200-220. */
      0,0,0,0,0,0,0,2,2,2,  /* Degrees 220-240. Camshaft is active from 146* to 506* (total = 360*). */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 240-260. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 260-280. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 280-300. Tooth #5 at 294* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 300-320. Tooth #6 at 314* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 320-340. Tooth #7 at 334* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 340-360. Tooth #8 at 354* for 2* duration. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 360-380. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 380-400. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 400-420. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 420-440. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 440-460. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 460-480. Tooth #9 at 474* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 480-500. Tooth #10 at 494* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 500-520. Tooth #11 at 514* for 2* duration. */
      2,2,2,2,2,2,3,2,2,2,  /* Degrees 520-540. Tooth #12 at 534* for 2* duration. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 540-560. */
      2,2,2,2,2,2,2,2,2,2,  /* Degrees 560-580. */
      2,2,2,2,2,2,2,0,0,0,  /* Degrees 580-600. Camshaft is down at 596. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 600-620. */
      0,0,0,0,0,0,0,0,0,0,  /* Degrees 620-640. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 640-660. Tooth #13 at 654* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 660-680. Tooth #14 at 674* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0,  /* Degrees 680-700. Tooth #15 at 694* for 2* duration. */
      0,0,0,0,0,0,1,0,0,0  /* Degrees 700-720. Tooth #16 at 714* for 2* duration. */
   };

   const unsigned char viper9602wheel[] PROGMEM = 
   // Viper pattern has 10 total crank teeth that are on shortly in pairs. Cam is high for 360* of crank then low for the next 360* of crank
   // This pattern was added by Dale Follett of Twisted Builds LLC going off a supplied oscilloscope image of the wheel pattern. Due to this
   // There is no guarentees on this wheel pattern as of 3/24/2024 and this pattern should be used at your own risk. However it should be correct.
   // I'm basing this using percentages. 120 total "edges" but should duplicate the factory wheels. Will test with o-scope.
   {
      //Cam on this revolution
      2,2,2,2,2,2,3,3,2,2, //1-5
      2,2,3,3,2,2,2,2,2,2, //6-10
      2,2,2,2,2,2,2,2,2,2, //11-15
      3,3,2,2,2,2,3,3,2,2, //16-20
      2,2,2,2,2,2,2,2,2,2, //21-25
      2,2,2,2,3,3,2,2,2,2, //26-30
      3,3,2,2,2,2,2,2,2,2, //31-35
      2,2,2,2,2,2,2,2,3,3, //36-40
      2,2,2,2,3,3,2,2,2,2, //41-45
      2,2,2,2,2,2,2,2,2,2, //46-50
      2,2,3,3,2,2,2,2,3,3, //51-55
      2,2,2,2,2,2,2,2,2,2, //56-60
      //Cam off this revolution
      0,0,0,0,0,0,1,1,0,0, //1-5
      0,0,1,1,0,0,0,0,0,0, //6-10
      0,0,0,0,0,0,0,0,0,0, //11-15
      1,1,0,0,0,0,1,1,0,0, //16-20
      0,0,0,0,0,0,0,0,0,0, //21-25
      0,0,0,0,1,1,0,0,0,0, //26-30
      1,1,0,0,0,0,0,0,0,0, //31-35
      0,0,0,0,0,0,0,0,1,1, //36-40
      0,0,0,0,1,1,0,0,0,0, //41-45
      0,0,0,0,0,0,0,0,0,0, //46-50
      0,0,1,1,0,0,0,0,1,1, //51-55
      0,0,0,0,0,0,0,0,0,0, //56-60
   };

 /* 36-2 with second trigger pulse across teeth 33-34 on first rotation */
 const unsigned char thirty_six_minus_two_with_second_trigger[] PROGMEM =
   { /* 36-2 + single tooth cam */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 1-5 */
     1,0,1,0,3,2,3,2,1,0, /* Teeth 6-10, cam in here somewhere - length/position not accurate to actual engine*/
     1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */
     1,0,1,0,1,0,1,0,0,0, /* Teeth 31-34, 35th tooth missing */
     0,0,                 /* 36th tooth MISSING */
     1,0,1,0,1,0,1,0,1,0, /* Revolution 2  Teeth 1-5 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 6-10 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 11-15 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 16-20 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 21-25 */
     1,0,1,0,1,0,1,0,1,0, /* Teeth 26-30 */
     1,0,1,0,1,0,1,0,0,0, /* Teeth 31-34, 35th tooth missing  */
     0,0                  /* 36th MISSING tooth */
   };

   // GM 40 tooth OSS wheel for transmission simulation. Simple on/off 40 teeth for 360* of rotation with no missing teeth.
   //Added by Dale Follett of Twisted Builds LLC 02-23-2025 for transmission controller simulation.
  const unsigned char GM40toothOSS[] PROGMEM = 
   {
      1,0,1,0,1,0,1,0,1,0, // Teeth 0-5
      1,0,1,0,1,0,1,0,1,0, // Teeth 6-10
      1,0,1,0,1,0,1,0,1,0, // Teeth 11-15
      1,0,1,0,1,0,1,0,1,0, // Teeth 16-20
      1,0,1,0,1,0,1,0,1,0, // Teeth 21-25
      1,0,1,0,1,0,1,0,1,0, // Teeth 26-30
      1,0,1,0,1,0,1,0,1,0, // Teeth 31-35
      1,0,1,0,1,0,1,0,1,0, // Teeth 36-40
   };

#endif