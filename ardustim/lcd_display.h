/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * I2C LCD Display Implementation
 * 
 * Concrete implementation of DisplayInterface for 16x2 I2C LCD
 * Provides error handling and graceful degradation
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "display_interface.h"
#include <LiquidCrystal_I2C.h>

/**
 * I2C LCD Display Configuration
 */
#define LCD_I2C_ADDRESS 0x3F   // Try 0x3F if 0x27 doesn't work
#define LCD_COLUMNS 20          // 20x4 LCD display
#define LCD_ROWS 4

/**
 * Concrete implementation of DisplayInterface for I2C LCD
 * Handles I2C communication errors and provides graceful fallback
 */
class LCDDisplay : public DisplayInterface {
private:
    LiquidCrystal_I2C* lcd;
    bool initialized;
    bool available;
    
public:
    /**
     * Constructor - creates LCD instance but doesn't initialize
     */
    LCDDisplay();
    
    /**
     * Destructor - cleanup LCD instance
     */
    ~LCDDisplay();
    
    /**
     * Initialize I2C LCD with error handling
     * @return true if initialization successful
     */
    bool init() override;
    
    /**
     * Clear display with error handling
     */
    void clear() override;
    
    /**
     * Set cursor position with bounds checking
     * @param col Column (0-15 for 16x2 LCD)
     * @param row Row (0-1 for 16x2 LCD)
     */
    void setCursor(uint8_t col, uint8_t row) override;
    
    /**
     * Print text with error handling
     * @param text String to display
     */
    void print(const char* text) override;
    
    /**
     * Print integer with error handling
     * @param value Integer to display
     */
    void print(int value) override;
    
    /**
     * Check if LCD is available and functional
     * @return true if LCD is working
     */
    bool isAvailable() override;
    

};

#endif