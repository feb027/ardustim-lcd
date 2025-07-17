/**
 * LCD Test for 20x4 Display
 * Tests the updated LCD functionality with new wheel names
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration for 20x4 display
#define LCD_I2C_ADDRESS 0x3F
#define LCD_COLUMNS 20
#define LCD_ROWS 4

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Display test pattern
  lcd.setCursor(0, 0);
  lcd.print("36-1");
  
  lcd.setCursor(0, 1);
  lcd.print("RPM: 1500");
  
  lcd.setCursor(0, 2);
  lcd.print("Mode: Fixed");
  
  lcd.setCursor(0, 3);
  lcd.print("Ready");
  
  Serial.println("LCD 20x4 test complete");
}

void loop() {
  // Test cycling through different wheel names
  static unsigned long lastUpdate = 0;
  static int wheelIndex = 0;
  
  const char* wheelNames[] = {
    "36-1",
    "60-2",
    "24-1",
    "GM LS1",
    "Mitsubishi 4G63",
    "Honda D17",
    "Miata 99-05",
    "Subaru 6/7",
    "BMW N20",
    "Dodge Viper V10"
  };
  
  if (millis() - lastUpdate > 3000) {
    lcd.setCursor(0, 0);
    lcd.print("                    "); // Clear line
    lcd.setCursor(0, 0);
    lcd.print(wheelNames[wheelIndex]);
    
    wheelIndex = (wheelIndex + 1) % 10;
    lastUpdate = millis();
  }
}
