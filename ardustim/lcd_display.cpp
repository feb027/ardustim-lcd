    /* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * I2C LCD Display Implementation
 * 
 * Concrete implementation of DisplayInterface for 16x2 I2C LCD
 * Provides error handling and graceful degradation
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */

#include "lcd_display.h"
#include <Wire.h>
#include <Arduino.h>

LCDDisplay::LCDDisplay() : lcd(nullptr), initialized(false), available(false) {
    // Create LCD instance with I2C address and dimensions
    lcd = new LiquidCrystal_I2C(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
}

LCDDisplay::~LCDDisplay() {
    if (lcd) {
        delete lcd;
        lcd = nullptr;
    }
}

bool LCDDisplay::init() {
    if (!lcd) {
        return false;
    }
    
    // Initialize I2C communication
    Wire.begin();
    
    // Initialize LCD
    lcd->init();
    lcd->backlight();
    lcd->clear();
    
    initialized = true;
    available = true;
    return true;
}

void LCDDisplay::clear() {
    if (!available || !lcd) {
        return;
    }
    lcd->clear();
}

void LCDDisplay::setCursor(uint8_t col, uint8_t row) {
    if (!available || !lcd) {
        return;
    }
    
    // Bounds checking for 16x2 LCD
    if (col >= LCD_COLUMNS) col = LCD_COLUMNS - 1;
    if (row >= LCD_ROWS) row = LCD_ROWS - 1;
    
    lcd->setCursor(col, row);
}

void LCDDisplay::print(const char* text) {
    if (!available || !lcd || !text) {
        return;
    }
    lcd->print(text);
}

void LCDDisplay::print(int value) {
    if (!available || !lcd) {
        return;
    }
    lcd->print(value);
}

bool LCDDisplay::isAvailable() {
    return available;
}