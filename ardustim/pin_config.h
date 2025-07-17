/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Pin Configuration for Ardu-Stim LCD Interface
 * 
 * Centralizes all pin assignments for LCD interface functionality
 * Arduino Nano pin assignments for buttons and I2C LCD
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__

/**
 * Button Pin Assignments for Arduino Nano
 * These pins are used for user interface control
 */
#define PIN_BUTTON_PREV     2   // D2 - Previous wheel pattern / RPM decrease (when held)
#define PIN_BUTTON_NEXT     3   // D3 - Next wheel pattern / RPM increase (when held)
#define PIN_BUTTON_SAVE     4   // D4 - Save current configuration to EEPROM
#define PIN_BUTTON_ABT      5   // D5 - Show status information (About)
#define PIN_BUTTON_HELP     6   // D6 - Cycle through RPM control modes

/**
 * I2C Pin Assignments for Arduino Nano
 * These are hardware-defined for I2C communication
 */
#define PIN_I2C_SDA         A4  // I2C Data line (Arduino Nano)
#define PIN_I2C_SCL         A5  // I2C Clock line (Arduino Nano)

/**
 * LCD I2C Configuration
 */
#define LCD_I2C_ADDRESS     0x27    // Common I2C address for LCD backpack modules
#define LCD_COLUMNS         16      // 16x2 character LCD display
#define LCD_ROWS            2

/**
 * Button Configuration Constants
 */
#define BUTTON_COUNT        5       // Total number of buttons
#define BUTTON_ACTIVE_LOW   true    // Buttons are active low (pressed = LOW)

/**
 * Timing Constants for Button Operations
 */
#define DEBOUNCE_DELAY_MS   50      // Button debounce time in milliseconds
#define REPEAT_DELAY_MS     500     // Time before button repeat starts
#define REPEAT_RATE_MS      150     // Time between button repeats

/**
 * Display Update Timing
 */
#define DISPLAY_REFRESH_MIN_MS  100     // Minimum time between display updates
#define MESSAGE_TIMEOUT_MS      1000    // Default message display timeout
#define STATUS_DISPLAY_MS       3000    // Status screen display duration

/**
 * Memory Constraints for Arduino Nano
 */
#define TARGET_FLASH_LIMIT      4096    // Target flash usage limit for LCD module (4KB)
#define TARGET_RAM_LIMIT        200     // Target RAM usage limit for LCD module (200 bytes)
#define NANO_TOTAL_FLASH        32768   // Arduino Nano total flash memory
#define NANO_TOTAL_RAM          2048    // Arduino Nano total RAM

#endif