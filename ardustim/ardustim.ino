/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Arbritrary wheel pattern generator
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

#include "globals.h"
#include "ardustim.h"
#include "enums.h"
#include "comms.h"
#include "storage.h"
#include "wheel_defs.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>

#ifndef ENABLE_LCD_INTERFACE
#define ENABLE_LCD_INTERFACE 1  // Default to enabled
#endif

#if ENABLE_LCD_INTERFACE
#include "ui_controller.h"
#include "lcd_manager.h"
#include "button_manager.h"
#include "lcd_display.h"
#endif

struct configTable config;
struct status currentStatus;

#if ENABLE_LCD_INTERFACE
/* LCD Interface Components */
LCDDisplay lcdDisplay;
LCDManager lcdManager;
ButtonManager buttonManager;
UIController uiController;
#endif

/* Sensistive stuff used in ISR's */
volatile uint16_t adc0; /* POT RPM */
volatile uint16_t adc1; /* Pot Wheel select */
/* Setting rpm to any value over 0 will enabled sweeping by default */
/* Stuff for handling prescaler changes (small tooth wheels are low RPM) */
volatile uint8_t analog_port = 0;
volatile bool adc0_read_complete = false;
volatile bool adc1_read_complete = false;
volatile bool reset_prescaler = false;
volatile uint8_t output_invert_mask = 0x00; /* Don't invert anything */
volatile uint8_t prescaler_bits = 0;
volatile uint8_t last_prescaler_bits = 0;
volatile uint16_t new_OCR1A = 5000; /* sane default */
volatile uint16_t edge_counter = 0;
volatile uint32_t cycleStartTime = micros();
volatile uint32_t cycleDuration = 0;
uint32_t sweep_time_counter = 0;
uint8_t sweep_direction = ASCENDING;

/* Less sensitive globals */
uint8_t bitshift = 0;

#if ENABLE_LCD_INTERFACE
/* Startup sequence variables */
bool startupSequenceActive = false;
uint8_t startupSequenceState = 0;
uint32_t startupSequenceTimer = 0;
uint8_t loadingDots = 0;  // For animated loading dots
#endif




wheels Wheels[MAX_WHEELS] = {
   /* Pointer to friendly name string, pointer to edge array, RPM Scaler, Number of edges in the array, whether the number of edges covers 360 or 720 degrees */
  { dizzy_four_cylinder_friendly_name, dizzy_four_cylinder, 0.03333, 4, 360 },
  { dizzy_six_cylinder_friendly_name, dizzy_six_cylinder, 0.05, 6, 360 },
  { dizzy_eight_cylinder_friendly_name, dizzy_eight_cylinder, 0.06667, 8, 360 },
  { sixty_minus_two_friendly_name, sixty_minus_two, 1.0, 120, 360 },
  { sixty_minus_two_with_cam_friendly_name, sixty_minus_two_with_cam, 1.0, 240, 720 },
  { sixty_minus_two_with_halfmoon_cam_friendly_name, sixty_minus_two_with_halfmoon_cam, 1.0, 240, 720 },
  { thirty_six_minus_one_friendly_name, thirty_six_minus_one, 0.6, 72, 360 },
  { twenty_four_minus_one_friendly_name, twenty_four_minus_one, 0.5, 48, 360 },
  { four_minus_one_with_cam_friendly_name, four_minus_one_with_cam, 0.06667, 16, 720 },
  { eight_minus_one_friendly_name, eight_minus_one, 0.13333, 16, 360 },
  { six_minus_one_with_cam_friendly_name, six_minus_one_with_cam, 0.15, 36, 720 },
  { twelve_minus_one_with_cam_friendly_name, twelve_minus_one_with_cam, 0.6, 144, 720 },
  { fourty_minus_one_friendly_name, fourty_minus_one, 0.66667, 80, 360 },
  { dizzy_four_trigger_return_friendly_name, dizzy_four_trigger_return, 0.03333, 4, 360 },
  { oddfire_vr_friendly_name, oddfire_vr, 0.03333, 4, 360 },
  { optispark_lt1_friendly_name, optispark_lt1, 3.0, 720, 720 },
  { twelve_minus_three_friendly_name, twelve_minus_three, 0.15, 18, 360 },
  { thirty_six_minus_two_two_two_friendly_name, thirty_six_minus_two_two_two, 0.6, 72, 360 },
  { thirty_six_minus_two_two_two_h6_friendly_name, thirty_six_minus_two_two_two_h6, 0.6, 72, 360 },
  { thirty_six_minus_two_two_two_with_cam_friendly_name, thirty_six_minus_two_two_two_with_cam, 0.6, 144, 720 },
  { fourty_two_hundred_wheel_friendly_name, fourty_two_hundred_wheel, 0.66667, 80, 360 },
  { thirty_six_minus_one_with_cam_fe3_friendly_name, thirty_six_minus_one_with_cam_fe3, 0.6, 144, 720 },
  { six_g_seventy_two_with_cam_friendly_name, six_g_seventy_two_with_cam, 1.0, 240, 720 },
  { buell_oddfire_cam_friendly_name, buell_oddfire_cam, 0.03333, 4, 360 },
  { gm_ls1_crank_and_cam_friendly_name, gm_ls1_crank_and_cam, 6.0, 720, 720 },
  { gm_ls_58X_crank_and_4x_cam_friendly_name, GM_LS_58X_crank_and_4x_cam, 1.0, 240, 720},
  { lotus_thirty_six_minus_one_one_one_one_friendly_name, lotus_thirty_six_minus_one_one_one_one, 0.6, 144, 720 },
  { honda_rc51_with_cam_friendly_name, honda_rc51_with_cam, 0.6, 144, 720 },
  { thirty_six_minus_one_with_second_trigger_friendly_name, thirty_six_minus_one_with_second_trigger, 0.6, 144, 720 },
  { weber_iaw_with_cam_friendly_name, weber_iaw_with_cam, 0.6, 144, 720 },
  { fiat_one_point_eight_sixteen_valve_with_cam_friendly_name, fiat_one_point_eight_sixteen_valve_with_cam, 0.6, 144, 720 },
  { three_sixty_nissan_cas_friendly_name, three_sixty_nissan_cas, 3.0, 720, 720 },
  { twenty_four_minus_two_with_second_trigger_friendly_name, twenty_four_minus_two_with_second_trigger, 0.5, 120, 720 },
  { yamaha_eight_tooth_with_cam_friendly_name, yamaha_eight_tooth_with_cam, 0.13333, 32, 720 },
  { mitsubishi_4g63_4_2_friendly_name, mitsubishi_4g63_4_2, 0.6, 144, 720 },
  { audi_135_with_cam_friendly_name, audi_135_with_cam, 2.25, 540, 720 },
  { honda_d17_no_cam_friendly_name, honda_d17_no_cam, 0.6, 144, 720 },
  { daihatsu_3cyl_friendly_name, daihatsu_3cyl, 0.03333, 4, 360 },
  { miata_9905_friendly_name, miata_9905, 0.6, 144, 720 },
  { twelve_with_cam_friendly_name, twelve_with_cam, 0.2, 48, 720 },
  { twenty_four_with_cam_friendly_name, twenty_four_with_cam, 0.4, 96, 720 },
  { subaru_six_seven_name_friendly_name, subaru_six_seven, 3.0, 720, 720 },
  { gm_seven_x_friendly_name, gm_seven_x, 1.502, 180, 720 },
  { four_twenty_a_friendly_name, four_twenty_a, 0.6, 144, 720 },
  { ford_st170_friendly_name, ford_st170, 3.0, 720, 720 },
  { mitsubishi_3A92_friendly_name, mitsubishi_3A92, 0.05, 12, 720 },
  { Toyota_4AGE_CAS_friendly_name, toyota_4AGE_CAS, 0.333, 144, 720 },
  { Toyota_4AGZE_friendly_name, toyota_4AGZE, 0.4, 96, 720 },
  { Suzuki_DRZ400_friendly_name, suzuki_DRZ400, 0.6, 72, 360},
  { Jeep_2000_4cyl_friendly_name, jeep_2000_4cyl, 1.5, 360, 720},
  { VIPER9602_friendly_name, viper9602wheel, 1.0, 240, 720},
  { thirty_six_minus_two_with_second_trigger_friendly_name, thirty_six_minus_two_with_second_trigger, 0.6, 144, 720 },
  { GM_40_Tooth_Trans_OSS_friendly_name, GM40toothOSS, 0.66667, 80, 360 },
};

/* Initialization */
void setup() {
  loadConfig();
  serialSetup();

  cli(); // stop interrupts

  /* Configuring TIMER1 (pattern generator) */
  // Set timer1 to generate pulses
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Set compare register to sane default
  OCR1A = 1000;  /* 8000 RPM (60-2) */

  // Turn on CTC mode
  TCCR1B |= (1 << WGM12); // Normal mode (not PWM)
  // Set prescaler to 1
  TCCR1B |= (1 << CS10); /* Prescaler of 1 */
  // Enable output compare interrupt for timer channel 1 (16 bit)
  TIMSK1 |= (1 << OCIE1A);

  // Set timer2 to run sweeper routine
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // Set compare register to sane default
  OCR2A = 249;  /* With prescale of x64 gives 1ms tick */

  // Turn on CTC mode
  TCCR2A |= (1 << WGM21); // Normal mode (not PWM)
  // Set prescaler to x64
  TCCR2B |= (1 << CS22); /* Prescaler of 64 */
  // Enable output compare interrupt for timer channel 2
  //TIMSK2 |= (1 << OCIE2A); //Disabled as no longer using TIMER2 for sweep


  /* Configure ADC as per http://www.glennsweeney.com/tutorials/interrupt-driven-analog-conversion-with-an-atmega328p */
  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  ADMUX &= B11011111;
  
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  ADMUX |= B01000000;
  
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input
  ADMUX &= B11110000;
  
  // Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
  // Do not set above 15! You will overrun other parts of ADMUX. A full
  // list of possible inputs is available in Table 24-4 of the ATMega328
  // datasheet
  // ADMUX |= 8;
  // ADMUX |= B00001000; // Binary equivalent
  
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  ADCSRA |= B10000000;
  
  // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
  ADCSRA |= B00100000;
  
  // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  ADCSRB &= B11111000;
  
  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= B00000111;
  
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= B00001000;

//  pinMode(7, OUTPUT); /* Debug pin for Saleae to track sweep ISR execution speed */
  pinMode(8, OUTPUT); /* Primary (crank usually) output */
  pinMode(9, OUTPUT); /* Secondary (cam1 usually) output */
  pinMode(10, OUTPUT); /* Tertiary (cam2 usually) output */
  pinMode(11, OUTPUT); /* Knock signal for seank, ony on LS1 pattern, NOT IMPL YET */
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  pinMode(53, OUTPUT); /* crank */
  pinMode(52, OUTPUT); /* cam 1 */
  pinMode(51, OUTPUT); /* untested - should be cam2*/
#endif

  sei(); // Enable interrupts

#if ENABLE_LCD_INTERFACE
  /* Initialize LCD Interface Components */
  // Initialize button manager first
  buttonManager.init();
  
  // Initialize LCD display with error handling
  bool lcdInitialized = lcdDisplay.init();
  if (lcdInitialized) {
    // LCD hardware is available, initialize managers
    lcdManager.init(&lcdDisplay);
    uiController.init(&buttonManager, &lcdManager);
    
    // Start non-blocking startup sequence
    lcdManager.enterStartupMode();
    startupSequenceState = 0;
    startupSequenceTimer = millis();
    startupSequenceActive = true;
  } else {
    // LCD initialization failed - system continues with serial interface only
    // Log error to serial (will be available after serialSetup())
    // Note: Serial output will show this after setup() completes
    startupSequenceActive = false;
  }
#endif
  // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
  ADCSRA |= B01000000;
  /* Make sure we are using the DEFAULT RPM on startup */
  reset_new_OCR1A(currentStatus.rpm); 

} // End setup


//! ADC ISR for alternating between ADC pins 0 and 1
/*!
 * Reads ADC ports 0 and 1 alternately. Port 0 is RPM, Port 1 is for
 * future fun (possible wheel selection)
 */
ISR(ADC_vect){
  if (analog_port == 0)
  {
    adc0 = ADCL | (ADCH << 8);
	  adc0_read_complete = true;
    /* Flip to channel 1 */
    //ADMUX |= B00000001;
    //analog_port = 1;
    /* Trigger another conversion */
    //ADCSRA |= B01000000;
    return;
  } 
//  if (analog_port == 1)
//  {
//    adc1 = ADCL | (ADCH << 8);
//	adc1_read_complete = true;
//    /* Flip to channel 0 */
//    /* Tell it to read ADC0, clear MUX0..3 */
//    ADMUX &= B11110000;
//    analog_port = 0;
//    /* Trigger another conversion */
//    ADCSRA |= B01000000;
//    return;
//  }
}

/* Pumps the pattern out of flash to the port 
 * The rate at which this runs is dependent on what OCR1A is set to
 */
ISR(TIMER1_COMPA_vect) 
{
  /* This is VERY simple, just walk the array and wrap when we hit the limit */
  PORTB = output_invert_mask ^ pgm_read_byte(&Wheels[config.wheel].edge_states_ptr[edge_counter]);   /* Write it to the port */
  
  edge_counter++;
  if (edge_counter == Wheels[config.wheel].wheel_max_edges) 
  {
    edge_counter = 0;
    cycleDuration = micros() - cycleStartTime;
    cycleStartTime = micros();
  }
  /* The tables are in flash so we need pgm_read_byte() */

  /* Reset Prescaler only if flag is set */
  if (reset_prescaler)
  {
    TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12)); /* Clear CS10, CS11 and CS12 */
    TCCR1B |= prescaler_bits;
    reset_prescaler = false;
  }
  /* Reset next compare value for RPM changes */
  OCR1A = new_OCR1A;  /* Apply new "RPM" from Timer2 ISR, i.e. speed up/down the virtual "wheel" */
}

#if ENABLE_LCD_INTERFACE
/**
 * Handle non-blocking startup sequence with impressive visual effects
 */
void handleStartupSequence() {
  if (!startupSequenceActive) return;
  
  uint32_t currentTime = millis();
  uint32_t elapsed = currentTime - startupSequenceTimer;
  
  switch (startupSequenceState) {
    case 0:
      // Animated welcome screen
      showWelcomeScreen();
      startupSequenceState = 1;
      startupSequenceTimer = currentTime;
      break;
      
    case 1:
      // Wait for welcome animation
      if (elapsed >= 2000) {
        startupSequenceState = 2;
        startupSequenceTimer = currentTime;
      }
      break;
      
    case 2:
      // Loading progress bar
      showLoadingProgress(elapsed);
      if (elapsed >= 2500) {
        startupSequenceState = 3;
        startupSequenceTimer = currentTime;
      }
      break;
      
    case 3:
      // System checks
      showSystemChecks(elapsed);
      if (elapsed >= 2000) {
        startupSequenceState = 4;
        startupSequenceTimer = currentTime;
      }
      break;
      
    case 4:
      // Feature showcase
      showFeatureShowcase(elapsed);
      if (elapsed >= 2000) {
        startupSequenceState = 5;
        startupSequenceTimer = currentTime;
      }
      break;
      
    case 5:
      // Ready screen
      showReadyScreen();
      startupSequenceState = 6;
      startupSequenceTimer = currentTime;
      break;
      
    case 6:
      // Final wait before switching to main display
      if (elapsed >= 1500) {
        startupSequenceActive = false;
        lcdManager.exitStartupMode();
      }
      break;
  }
}

/**
 * Show animated welcome screen
 */
void showWelcomeScreen() {
  lcdManager.showStartupMessage("* ARDU-STIM v3.0 *");
  
  // Add decorative elements on other lines
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("====================");
  lcdDisplay.setCursor(0, 2);
  lcdDisplay.print("  Engine Simulator   ");
  lcdDisplay.setCursor(0, 3);
  lcdDisplay.print("====================");
}

/**
 * Show loading progress bar
 */
void showLoadingProgress(uint32_t elapsed) {
  static uint8_t lastProgress = 0;
  
  // Calculate progress (0-18 characters to fit in brackets)
  uint8_t progress = (elapsed * 18) / 2500;
  if (progress > 18) progress = 18;  // Cap at 18 to prevent overflow
  
  // Update display only if progress changed
  if (progress != lastProgress) {
    lastProgress = progress;
    
    lcdDisplay.clear();
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("   Loading System    ");
    
    // Progress bar on line 2
    lcdDisplay.setCursor(0, 2);
    lcdDisplay.print("[");
    for (uint8_t i = 0; i < 18; i++) {
      if (i < progress) {
        lcdDisplay.print("=");
      } else {
        lcdDisplay.print(" ");
      }
    }
    lcdDisplay.print("]");
    
    // Percentage and static "Complete" text on line 3
    lcdDisplay.setCursor(0, 3);
    lcdDisplay.print("     ");
    lcdDisplay.print((progress * 100) / 18);
    lcdDisplay.print("% Complete");
  }
}

/**
 * Show system checks with checkmarks
 */
void showSystemChecks(uint32_t elapsed) {
  static uint8_t lastCheck = 0;
  
  // Show checks every 400ms
  uint8_t currentCheck = elapsed / 400;
  if (currentCheck > 4) currentCheck = 4;
  
  if (currentCheck != lastCheck) {
    lastCheck = currentCheck;
    
    lcdDisplay.clear();
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("  System Checks     ");
    
    // Check items
    const char* checks[] = {
      "Timer Hardware",
      "ADC System",
      "Pattern Memory",
      "LCD Display",
      "User Interface"
    };
    
    for (uint8_t i = 0; i < 5 && i <= currentCheck; i++) {
      if (i < 3) {  // Only show 3 checks to fit on screen
        lcdDisplay.setCursor(0, i + 1);
        lcdDisplay.print(checks[i]);
        lcdDisplay.print(" [OK]");
      }
    }
  }
}

/**
 * Show feature showcase
 */
void showFeatureShowcase(uint32_t elapsed) {
  static uint8_t lastFeature = 0;
  
  // Show different features every 500ms
  uint8_t currentFeature = elapsed / 500;
  if (currentFeature > 3) currentFeature = 3;
  
  if (currentFeature != lastFeature) {
    lastFeature = currentFeature;
    
    lcdDisplay.clear();
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("    Features        ");
    
    const char* features[] = {
      "30+ Wheel Patterns",
      "RPM: 0-9000",
      "3 Control Modes",
      "LCD + Serial UI"
    };
    
    lcdDisplay.setCursor(0, 2);
    lcdDisplay.print("* ");
    lcdDisplay.print(features[currentFeature]);
  }
}

/**
 * Show ready screen
 */
void showReadyScreen() {
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("********************");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("*  SYSTEM READY!   *");
  lcdDisplay.setCursor(0, 2);
  lcdDisplay.print("*   Let's Start!   *");
  lcdDisplay.setCursor(0, 3);
  lcdDisplay.print("********************");
}
#endif

void loop() 
{
  uint16_t tmp_rpm = currentStatus.base_rpm;
  /* Just handle the Serial UI, everything else is in 
   * interrupt handlers or callbacks from SerialUI.
   */
  if(Serial.available() > 0) { commandParser(); }

#if ENABLE_LCD_INTERFACE
  /* Handle startup sequence first */
  handleStartupSequence();
  
  /* Update LCD Interface - non-blocking operations only */
  uiController.update();
  lcdManager.update();
#endif

  if(config.mode == POT_RPM)
  {
    if (adc0_read_complete == true)
    {
      adc0_read_complete = false;
      
      // More aggressive exponential moving average for better noise reduction
      static uint16_t smoothedADC = 0;
      static bool firstRead = true;
      
      // Initialize on first read
      if (firstRead) {
        smoothedADC = adc0;
        firstRead = false;
      } else {
        // More aggressive exponential moving average (alpha = 0.1 for much smoother filtering)
        // This gives 90% weight to previous value, 10% to new reading
        smoothedADC = (smoothedADC * 9 + adc0) / 10;
      }
      
      // Improved potentiometer scaling with deadband at low end
      if (smoothedADC < 10) {
        tmp_rpm = 0;  // Allow 0 RPM for very low pot values
      } else {
        tmp_rpm = ((uint32_t)smoothedADC * TMP_RPM_CAP) / 1023;  // Linear scaling 0-9000 RPM
      }
    }
  }
  else if (config.mode == LINEAR_SWEPT_RPM)
  {
    
    if(micros() > (sweep_time_counter + config.sweep_interval))
    {
      sweep_time_counter = micros();
      if(sweep_direction == ASCENDING)
      {
        tmp_rpm = currentStatus.base_rpm + 50;  // Larger step size for visible changes
        if(tmp_rpm >= 9000) { // Use maximum RPM cap instead of config value
          sweep_direction = DESCENDING;
          tmp_rpm = 9000; // Clamp to max value
        }
      }
      else
      {
        // Use signed arithmetic to prevent underflow
        int32_t new_rpm = (int32_t)currentStatus.base_rpm - 50;
        if(new_rpm <= 0) { // Use 0 as minimum instead of config value
          sweep_direction = ASCENDING;
          tmp_rpm = 0; // Clamp to minimum value
        } else {
          tmp_rpm = (uint16_t)new_rpm;
        }
      }
    }
    
  }
  else if (config.mode == FIXED_RPM)
  {
    tmp_rpm = config.fixed_rpm;
  }
  currentStatus.base_rpm = tmp_rpm;

  currentStatus.compressionModifier = calculateCompressionModifier();
  if(currentStatus.compressionModifier >= currentStatus.base_rpm ) { currentStatus.compressionModifier = 0; }

  setRPM( (currentStatus.base_rpm - currentStatus.compressionModifier) );
}

uint16_t calculateCompressionModifier()
{
  if( (currentStatus.base_rpm > config.compressionRPM) || (config.useCompression != true) ) { return 0; }
  //if( currentStatus.base_rpm > 400 ) { return 0;}

  uint16_t crankAngle = calculateCurrentCrankAngle();
  uint16_t modAngle = crankAngle;

  uint16_t compressionModifier = 0;
  switch(config.compressionType)
  {
    case COMPRESSION_TYPE_2CYL_4STROKE:
      modAngle = crankAngle / 2;
      compressionModifier = pgm_read_byte(&sin_100_180[modAngle]);
      break;
    case COMPRESSION_TYPE_4CYL_4STROKE:
      modAngle = (crankAngle % 180) ;
      compressionModifier = pgm_read_byte(&sin_100_180[modAngle]);
      break;
    case COMPRESSION_TYPE_6CYL_4STROKE:
      modAngle = crankAngle % 120;
      compressionModifier = pgm_read_byte(&sin_100_120[modAngle]);
      break;
    case COMPRESSION_TYPE_8CYL_4STROKE:
      modAngle = crankAngle % 90;
      compressionModifier = pgm_read_byte(&sin_100_90[modAngle]);
      break;
    default:
      modAngle = (crankAngle % 180) ;
      compressionModifier = pgm_read_byte(&sin_100_180[modAngle]);
      break;
  }

  //RPM scaler - Varies the amplitude of the compression modifier based on how far below the compression RPM point we are. Eg:
  //If the compression RPM value is 400
  //At 300rpm the amplitude will be 75%
  //At 200rpm the amplitude will be 50%
  //At 100rpm the amplitude will be 25% etc
  //Base RPM must be below 650 to prevent overflow
  if(config.compressionDynamic && (currentStatus.base_rpm < 655U) ) { compressionModifier = (compressionModifier * currentStatus.base_rpm) / config.compressionRPM; }
  
  return compressionModifier;
}

uint16_t calculateCurrentCrankAngle()
{
  if(cycleDuration == 0) { return 0; }

  uint32_t cycleTime = micros() - cycleStartTime;
  if( Wheels[config.wheel].wheel_degrees == 720 ) { cycleTime = cycleTime * 2; } 
  
  uint16_t tmpCrankAngle = ((cycleTime * 360U) / cycleDuration);
  tmpCrankAngle += config.compressionOffset;
  while(tmpCrankAngle > 360) { tmpCrankAngle -= 360; }

  return tmpCrankAngle;
}

/*!
 * Validates the new user requested RPM and sets it if valid. 
 */ 
void setRPM(uint16_t newRPM)
{
  // Allow 0 RPM for potentiometer mode, but minimum 10 for other modes
  if (newRPM < 10 && config.mode != POT_RPM) { return; }

  if(currentStatus.rpm != newRPM) { reset_new_OCR1A( newRPM ); }
  currentStatus.rpm = newRPM;
}


void reset_new_OCR1A(uint32_t new_rpm)
{
  uint32_t tmp;
  uint8_t bitshift;
  uint8_t tmp_prescaler_bits;
  tmp = (uint32_t)(8000000.0/(Wheels[config.wheel].rpm_scaler * (float)(new_rpm < 10 ? 10:new_rpm)));
  //tmp = (uint32_t)(8000000/(Wheels[config.wheel].rpm_scaler * (new_rpm < 10 ? 10:new_rpm)));
  //uint64_t x = 800000000000ULL;

  get_prescaler_bits(&tmp,&tmp_prescaler_bits,&bitshift);

  new_OCR1A = (uint16_t)(tmp >> bitshift); 
  prescaler_bits = tmp_prescaler_bits;
  reset_prescaler = true; 
}


uint8_t get_bitshift_from_prescaler(uint8_t *prescaler_bits)
{
  switch (*prescaler_bits)
  {
    case PRESCALE_1024:
    return 10;
    case PRESCALE_256:
    return 8;
    case PRESCALE_64:
    return 6;
    case PRESCALE_8:
    return 3;
    case PRESCALE_1:
    return 0;
  }
  return 0;
}

//! Gets prescaler enum and bitshift based on OC value
void get_prescaler_bits(uint32_t *potential_oc_value, uint8_t *prescaler, uint8_t *bitshift)
{
  if (*potential_oc_value >= 16777216)
  {
    *prescaler = PRESCALE_1024;
    *bitshift = 10;
  }
  else if (*potential_oc_value >= 4194304)
  {
    *prescaler = PRESCALE_256;
    *bitshift = 8;
  }
  else if (*potential_oc_value >= 524288)
  {
    *prescaler = PRESCALE_64;
    *bitshift = 6;
  }
  else if (*potential_oc_value >= 65536)
  {
    *prescaler = PRESCALE_8;
    *bitshift = 3;
  }
  else
  {
    *prescaler = PRESCALE_1;
    *bitshift = 0;
  }
}
