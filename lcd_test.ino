// Simple LCD Test Program
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Try different addresses - change this based on your I2C scanner results
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your LCD's address

void setup() {
  Serial.begin(115200);
  Serial.println("LCD Test Starting...");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Test display
  lcd.setCursor(0, 0);
  lcd.print("LCD Test");
  lcd.setCursor(0, 1);
  lcd.print("Hello World!");
  
  Serial.println("LCD initialized and text sent");
}

void loop() {
  // Blink test
  lcd.backlight();
  delay(1000);
  lcd.noBacklight();
  delay(1000);
}