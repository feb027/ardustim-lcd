/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * LCD Display Manager Implementation
 * 
 * Coordinates display updates, manages screen states, and handles refresh logic
 * Supports Bahasa Indonesia text with English wheel pattern names
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */

#include "lcd_manager.h"
#include "globals.h"
#include "enums.h"
#include "wheel_defs.h"
#include <Arduino.h>
#include <avr/pgmspace.h>

// Minimal text constants to save flash
const char LCD_TEXT_SAVED[] PROGMEM = "SAVED";
const char LCD_TEXT_SAVING[] PROGMEM = "SAVING...";

// External wheel definitions
extern wheels Wheels[];

LCDManager::LCDManager() : display(nullptr), currentMode(DISPLAY_MAIN), 
                           messageTimeout(0), lastRefresh(0),
                           needsRefresh(true), forceRefreshFlag(false),
                           lastWheel(255), lastRPM(0), lastMode(255) {
    messageBuffer[0] = '\0';
}

void LCDManager::init(DisplayInterface* disp) {
    display = disp;
    currentMode = DISPLAY_MAIN;
    messageTimeout = 0;
    lastRefresh = 0;
    needsRefresh = true;
    forceRefreshFlag = false;
    
    // Initialize cached values to force first update
    lastWheel = 255;
    lastRPM = 0;
    lastMode = 255;
    
    messageBuffer[0] = '\0';
}

void LCDManager::update() {
    if (!display || !display->isAvailable()) {
        return;
    }
    
    uint32_t currentTime = millis();
    
    // Handle message timeout
    if (currentMode == DISPLAY_MESSAGE && messageTimeout > 0 && currentTime >= messageTimeout) {
        currentMode = DISPLAY_MAIN;
        messageTimeout = 0;
        needsRefresh = true;
    }
    

    
    // Check if state has changed
    if (hasStateChanged()) {
        needsRefresh = true;
    }
    
    // Limit refresh rate
    if (needsRefresh && (currentTime - lastRefresh >= DISPLAY_REFRESH_MIN || forceRefreshFlag)) {
        switch (currentMode) {
            case DISPLAY_MAIN:
                updateMainDisplay();
                break;
            case DISPLAY_MESSAGE:
                updateMessageDisplay();
                break;
        }
        
        needsRefresh = false;
        forceRefreshFlag = false;
        lastRefresh = currentTime;
    }
}

bool LCDManager::hasStateChanged() {
    // Check if any relevant values have changed
    bool changed = false;
    
    if (config.wheel != lastWheel) {
        lastWheel = config.wheel;
        changed = true;
    }
    
    // Only update RPM if it changed significantly to reduce flicker
    // Use different thresholds based on mode
    uint16_t rpmThreshold = 100;
    if (config.mode == POT_RPM) {
        rpmThreshold = 800; // Very large threshold for POT mode to prevent flickering
    } else if (config.mode == LINEAR_SWEPT_RPM) {
        rpmThreshold = 1000; // Extremely large threshold for sweep mode to prevent flickering
    }
    
    if (abs((int)currentStatus.rpm - (int)lastRPM) > rpmThreshold) {
        lastRPM = currentStatus.rpm;
        changed = true;
    }
    
    if (config.mode != lastMode) {
        lastMode = config.mode;
        changed = true;
    }
    
    return changed;
}

void LCDManager::updateMainDisplay() {
    if (!display) return;
    
    display->clear();
    
    // Line 1: Wheel pattern name - now can use full 20 characters
    char buffer[21]; // Increased buffer size for 20x4 display
    
    // Get wheel name and truncate to fit full 20 characters
    getWheelName(config.wheel, buffer, sizeof(buffer));
    display->setCursor(0, 0);
    display->print(buffer);
    
    // Line 2: Wheel number and additional wheel info
    display->setCursor(0, 1);
    
    // Show wheel number and degrees (360 or 720)
    uint16_t wheelDegrees = Wheels[config.wheel].wheel_degrees;
    snprintf(buffer, sizeof(buffer), "Pattern #%d (%d\xDF)", config.wheel + 1, wheelDegrees);
    display->print(buffer);
    
    // Line 3: RPM with more space and additional info
    display->setCursor(0, 2);
    display->print("RPM: ");
    
    // Format and display RPM (full numbers, no "k" suffix)
    formatRPM(currentStatus.rpm, buffer, sizeof(buffer));
    display->setCursor(5, 2);  // After "RPM: "
    display->print(buffer);
    
    // Add RPM scaling factor on same line
    float rpmScaler = Wheels[config.wheel].rpm_scaler;
    snprintf(buffer, sizeof(buffer), " (x%.2f)", rpmScaler);
    display->print(buffer);
    
    // Line 4: Mode with more descriptive text
    display->setCursor(0, 3);
    display->print("Mode: ");
    
    // Display mode with more descriptive text
    formatMode(config.mode, buffer, sizeof(buffer));
    display->setCursor(6, 3);  // Position for mode
    display->print(buffer);
}



void LCDManager::updateMessageDisplay() {
    if (!display) return;
    
    display->clear();
    
    // Center the message on line 2 (middle of 4 lines)
    uint8_t messageLen = strlen(messageBuffer);
    uint8_t startPos = (20 - messageLen) / 2;
    
    // Display message on line 2 (centered)
    display->setCursor(startPos, 1);
    display->print(messageBuffer);
    
    // Add ArduStim branding on line 4
    display->setCursor(3, 3);
    display->print("ArduStim v2.0");
}

void LCDManager::formatRPM(uint16_t rpm, char* buffer, uint8_t bufferSize) {
    // Always show full RPM numbers, no "k" suffix
    snprintf(buffer, bufferSize, "%d", rpm);
}

void LCDManager::formatMode(uint8_t mode, char* buffer, uint8_t bufferSize) {
    switch (mode) {
        case FIXED_RPM:
            strncpy(buffer, "Fixed RPM", bufferSize - 1);
            break;
        case POT_RPM:
            strncpy(buffer, "Potentiometer", bufferSize - 1);
            break;
        case LINEAR_SWEPT_RPM:
            strncpy(buffer, "Sweep", bufferSize - 1);
            break;
        default:
            strncpy(buffer, "Unknown", bufferSize - 1);
            break;
    }
    buffer[bufferSize - 1] = '\0';
}

void LCDManager::getWheelName(uint8_t wheelIndex, char* buffer, uint8_t bufferSize) {
    if (wheelIndex >= MAX_WHEELS) {
        strncpy(buffer, "Unknown", bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return;
    }
    
    // Get short wheel name from PROGMEM
    char* wheelNamePtr = (char*)pgm_read_word(&short_wheel_names[wheelIndex]);
    strncpy_P(buffer, wheelNamePtr, bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}



void LCDManager::showMessage(const char* message, uint16_t duration) {
    if (!display || !message) return;
    
    // Display message immediately (no queue)
    strncpy(messageBuffer, message, sizeof(messageBuffer) - 1);
    messageBuffer[sizeof(messageBuffer) - 1] = '\0';
    
    currentMode = DISPLAY_MESSAGE;
    messageTimeout = millis() + duration;
    needsRefresh = true;
}



void LCDManager::forceRefresh() {
    forceRefreshFlag = true;
    needsRefresh = true;
}

void LCDManager::returnToMain() {
    currentMode = DISPLAY_MAIN;
    messageTimeout = 0;
    needsRefresh = true;
}

bool LCDManager::isDisplayAvailable() {
    return display && display->isAvailable();
}