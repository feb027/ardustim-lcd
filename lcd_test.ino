/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * LCD Display Test for ArduStim
 * 
 * Tests the 20x4 LCD display with wheel names
 * Cycles through all wheel patterns to verify display
 */

#include "ardustim/lcd_display.h"
#include "ardustim/wheel_names.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Create LCD instance
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println(F("ArduStim LCD Test"));
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print(F("ArduStim LCD Test"));
  lcd.setCursor(0, 1);
  lcd.print(F("20x4 Display"));
  lcd.setCursor(0, 2);
  lcd.print(F("Testing wheel names"));
  lcd.setCursor(0, 3);
  lcd.print(F("Starting in 3s..."));
  
  delay(3000);
}

void loop() {
  // Cycle through all wheel patterns
  for (uint8_t i = 0; i < MAX_WHEELS; i++) {
    lcd.clear();
    
    // Display wheel number
    lcd.setCursor(0, 0);
    lcd.print(F("Pattern #"));
    lcd.print(i + 1);
    
    // Display wheel name
    lcd.setCursor(0, 1);
    
    // Get wheel name from PROGMEM
    char buffer[21];
    char* wheelNamePtr = (char*)pgm_read_word(&short_wheel_names[i]);
    strncpy_P(buffer, wheelNamePtr, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    lcd.print(buffer);
    
    // Display additional info
    lcd.setCursor(0, 3);
    lcd.print(F("Press any button..."));
    
    // Log to serial
    Serial.print(F("Testing wheel #"));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.println(buffer);
    
    // Wait before showing next wheel
    delay(2000);
  }
  
  // Show completion message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Test Complete"));
  lcd.setCursor(0, 1);
  lcd.print(F("All wheels displayed"));
  lcd.setCursor(0, 3);
  lcd.print(F("Restarting in 3s..."));
  
  delay(3000);
}