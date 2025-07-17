/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * User Interface Controller
 * 
 * Coordinates button actions with system state changes and display updates
 * Implements state machine for different UI modes
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __UI_CONTROLLER_H__
#define __UI_CONTROLLER_H__

#include <stdint.h>
#include <Arduino.h>
#include "button_manager.h"
#include "lcd_manager.h"
#include "globals.h"
#include "enums.h"

/**
 * UI State Machine States
 */
enum UIState {
    UI_STATE_NORMAL = 0,        // Normal operation mode
    UI_STATE_SAVING = 1         // Configuration save in progress
};

/**
 * RPM Adjustment Configuration
 */
#define RPM_INCREMENT           100     // RPM increment/decrement step
#define RPM_MIN                 10      // Minimum RPM value
#define RPM_MAX                 9000    // Maximum RPM value

/**
 * User Interface Controller Class
 * Coordinates all UI interactions between buttons, display, and system state
 */
class UIController {
private:
    ButtonManager* buttons;
    LCDManager* lcdManager;
    UIState currentState;
    uint32_t stateTimeout;
    bool initialized;
    
    // State tracking for change detection
    uint8_t lastWheel;
    uint16_t lastRPM;
    uint8_t lastMode;
    
    /**
     * Handle wheel pattern selection (NEXT/PREV buttons)
     * Implements wraparound logic for first/last patterns
     */
    void handleWheelSelection();
    
    /**
     * Handle RPM adjustment when buttons are held
     * Manages RPM increment/decrement with bounds checking
     */
    void handleRPMAdjustment();
    
    /**
     * Handle RPM mode cycling (HELP button)
     * Cycles through FIXED, POT, SWEEP modes
     */
    void handleModeChange();
    
    /**
     * Handle configuration save (SAVE button)
     * Shows progress and result messages
     */
    void handleSave();
    

    
    /**
     * Update UI state machine
     * Manages state transitions and timeouts
     */
    void updateStateMachine();
    
    /**
     * Check if system state has changed
     * Used to trigger display updates
     * @return true if state changed
     */
    bool hasSystemStateChanged();
    
    /**
     * Update cached state values
     * Called after processing state changes
     */
    void updateCachedState();
    
    // Removed helper functions to save flash memory - inlined in implementation
    
    /**
     * Check if it's safe to modify system state
     * Ensures UI actions don't interfere with critical timing
     * @return true if safe to modify state
     */
    bool isSafeToModifyState();
    
public:
    /**
     * Constructor - initializes UI controller state
     */
    UIController();
    
    /**
     * Initialize UI controller with button and LCD managers
     * @param btn Pointer to button manager instance
     * @param lcd Pointer to LCD manager instance
     */
    void init(ButtonManager* btn, LCDManager* lcd);
    
    /**
     * Update UI controller - call regularly in main loop
     * Processes button actions and manages UI state
     */
    void update();
    
    /**
     * Get current UI state
     * @return Current UIState
     */
    UIState getCurrentState();
    
    /**
     * Force return to normal state
     * Cancels any temporary UI modes
     */
    void returnToNormal();
    
    /**
     * Check if UI is currently processing actions
     * @return true if UI is busy (saving, adjusting, etc.)
     */
    bool isBusy();
    
    /**
     * Emergency stop - halt all UI processing
     * Used for critical system states
     */
    void emergencyStop();
    

};

#endif