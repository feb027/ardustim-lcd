/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * LCD Display Manager
 * 
 * Coordinates display updates, manages screen states, and handles refresh logic
 * Supports Bahasa Indonesia text with English wheel pattern names
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __LCD_MANAGER_H__
#define __LCD_MANAGER_H__

#include <stdint.h>
#include <avr/pgmspace.h>
#include "display_interface.h"

/**
 * Display Mode Enumeration
 */
enum DisplayMode {
    DISPLAY_MAIN = 0,       // Main screen: wheel name, RPM, mode
    DISPLAY_MESSAGE = 1,    // Temporary message display
    DISPLAY_STARTUP = 2     // Startup sequence display
};

/**
 * Display Timing Configuration
 */
#define MESSAGE_TIMEOUT_SHORT   1000    // 1 second for success messages
#define MESSAGE_TIMEOUT_LONG    2000    // 2 seconds for error messages
#define STATUS_DISPLAY_TIME     3000    // 3 seconds for status display
#define DISPLAY_REFRESH_MIN     1000     // Minimum 100ms between refreshes for smooth sweep mode

// Removed text constants to save flash memory - using direct strings

/**
 * LCD Manager Class
 * Handles all display coordination and state management
 */
class LCDManager {
private:
    DisplayInterface* display;
    DisplayMode currentMode;
    uint32_t messageTimeout;
    uint32_t lastRefresh;
    bool needsRefresh;
    bool forceRefreshFlag;
    bool forceStaticLabelReset;  // Force static labels to be reinitialized
    
    // Cached values for change detection
    uint8_t lastWheel;
    uint16_t lastRPM;
    uint8_t lastMode;
    bool isDisplayInitialized;  // Track if display has been initialized
    bool messageModeInitialized;  // Track if message mode has been initialized
    
    // Message buffer for temporary displays
    char messageBuffer[17];  // 16 chars + null terminator for LCD
    
    /**
     * Update main display screen
     * Shows wheel pattern, RPM, and mode
     */
    void updateMainDisplay();
    
    /**
     * Update message display screen
     * Shows temporary messages
     */
    void updateMessageDisplay();
    
    /**
     * Check if any values have changed since last update
     * @return true if display needs refresh
     */
    bool hasStateChanged();
    
    /**
     * Format RPM value for display
     * @param rpm RPM value to format
     * @param buffer Buffer to store formatted string
     * @param bufferSize Size of buffer
     */
    void formatRPM(uint16_t rpm, char* buffer, uint8_t bufferSize);
    
    /**
     * Format mode text in Indonesian
     * @param mode Mode value (from enums.h)
     * @param buffer Buffer to store formatted string
     * @param bufferSize Size of buffer
     */
    void formatMode(uint8_t mode, char* buffer, uint8_t bufferSize);
    
    /**
     * Get wheel pattern name from PROGMEM
     * @param wheelIndex Index of wheel pattern
     * @param buffer Buffer to store wheel name
     * @param bufferSize Size of buffer
     */
    void getWheelName(uint8_t wheelIndex, char* buffer, uint8_t bufferSize);
    
public:
    /**
     * Constructor - initializes LCD manager state
     */
    LCDManager();
    
    /**
     * Initialize LCD manager with display interface
     * @param disp Pointer to display interface implementation
     */
    void init(DisplayInterface* disp);
    
    /**
     * Update display - call regularly in main loop
     * Handles mode timeouts and refresh logic
     */
    void update();
    
    /**
     * Show temporary message with timeout
     * @param message Message text to display
     * @param duration Duration in milliseconds
     */
    void showMessage(const char* message, uint16_t duration);
    
    /**
     * Enter startup display mode
     * Prevents main display from interfering with startup sequence
     */
    void enterStartupMode();
    
    /**
     * Show startup message without timeout interference
     * @param message Message text to display
     */
    void showStartupMessage(const char* message);
    
    /**
     * Exit startup mode and return to main display
     */
    void exitStartupMode();

    
    /**
     * Force immediate display refresh
     * Useful after configuration changes
     */
    void forceRefresh();
    
    /**
     * Return to main display mode
     * Cancels any temporary displays
     */
    void returnToMain();
    
    /**
     * Check if display is currently available
     * @return true if display hardware is functional
     */
    bool isDisplayAvailable();
};

#endif