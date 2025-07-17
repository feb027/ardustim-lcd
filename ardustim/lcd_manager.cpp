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
    
    // Line 1: Wheel pattern name (no "Pola:" prefix)
    char buffer[17];
    display->setCursor(0, 0);
    
    // Get wheel name and truncate to fit full 16 characters
    getWheelName(config.wheel, buffer, sizeof(buffer));
    display->print(buffer);
    
    // Line 2: RPM and Mode
    display->setCursor(0, 1);
    display->print("RPM:");
    
    // Format and display RPM (full numbers, no "k" suffix)
    formatRPM(currentStatus.rpm, buffer, sizeof(buffer));
    display->setCursor(4, 1);  // After "RPM:"
    display->print(buffer);
    
    // Display mode
    formatMode(config.mode, buffer, sizeof(buffer));
    display->setCursor(12, 1);  // Position for mode
    display->print(buffer);
}



void LCDManager::updateMessageDisplay() {
    if (!display) return;
    
    display->clear();
    
    // Center the message on line 1
    uint8_t messageLen = strlen(messageBuffer);
    uint8_t startPos = (16 - messageLen) / 2;
    
    display->setCursor(startPos, 0);
    display->print(messageBuffer);
}

void LCDManager::formatRPM(uint16_t rpm, char* buffer, uint8_t bufferSize) {
    // Always show full RPM numbers, no "k" suffix
    snprintf(buffer, bufferSize, "%d", rpm);
}

void LCDManager::formatMode(uint8_t mode, char* buffer, uint8_t bufferSize) {
    switch (mode) {
        case FIXED_RPM:
            strncpy(buffer, "FIX", bufferSize - 1);
            break;
        case POT_RPM:
            strncpy(buffer, "POT", bufferSize - 1);
            break;
        case LINEAR_SWEPT_RPM:
            strncpy(buffer, "SWP", bufferSize - 1);
            break;
        default:
            strncpy(buffer, "?", bufferSize - 1);
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
    
    // Simple approach - just show wheel number for now to avoid PROGMEM issues
    snprintf(buffer, bufferSize, "Wheel %d", wheelIndex + 1);
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