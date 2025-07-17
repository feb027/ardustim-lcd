/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * User Interface Controller Implementation
 * 
 * Coordinates button actions with system state changes and display updates
 * Implements state machine for different UI modes
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */

#include "ui_controller.h"
#include "wheel_defs.h"
#include "storage.h"
#include <avr/pgmspace.h>

// External references to global variables and functions
extern struct configTable config;
extern struct status currentStatus;
extern wheels Wheels[];
extern void saveConfig();

// Removed text constants to save flash memory - using direct strings

UIController::UIController() {
    buttons = nullptr;
    lcdManager = nullptr;
    currentState = UI_STATE_NORMAL;
    stateTimeout = 0;
    initialized = false;
    
    // Initialize cached state
    lastWheel = 0;
    lastRPM = 0;
    lastMode = 0;
}

void UIController::init(ButtonManager* btn, LCDManager* lcd) {
    buttons = btn;
    lcdManager = lcd;
    
    if (buttons && lcdManager) {
        initialized = true;
        
        // Initialize cached state with current values
        lastWheel = config.wheel;
        lastRPM = currentStatus.rpm;
        lastMode = config.mode;
        
        currentState = UI_STATE_NORMAL;
        stateTimeout = 0;
    }
}

void UIController::update() {
    if (!initialized || !buttons || !lcdManager) {
        return;
    }
    
    // Update button states first
    buttons->update();
    
    // Only process UI actions if it's safe to do so
    if (!isSafeToModifyState()) {
        return;
    }
    
    // Update state machine
    updateStateMachine();
    
    // Process button actions based on current state
    switch (currentState) {
        case UI_STATE_NORMAL:
            handleWheelSelection();
            handleModeChange();
            handleSave();
            handleRPMAdjustment(); // Simplified - no separate state
            break;
            
        case UI_STATE_SAVING:
            // Saving state is handled by timeout in updateStateMachine()
            break;
    }
    
    // Update cached state if system state changed
    if (hasSystemStateChanged()) {
        updateCachedState();
        lcdManager->forceRefresh();
    }
}

void UIController::updateStateMachine() {
    uint32_t currentTime = millis();
    
    switch (currentState) {
        case UI_STATE_SAVING:
            if (currentTime >= stateTimeout) {
                currentState = UI_STATE_NORMAL;
            }
            break;
            
        default:
            // No timeout handling needed for other states
            break;
    }
}

void UIController::handleWheelSelection() {
    if (currentState != UI_STATE_NORMAL) {
        return;
    }
    
    // Only change wheels in POT mode (other modes use buttons for RPM)
    if (config.mode != POT_RPM) {
        return;
    }
    
    bool wheelChanged = false;
    
    // Handle NEXT button for next wheel pattern
    if (buttons->isPressed(BUTTON_NEXT)) {
        config.wheel = (config.wheel + 1 >= MAX_WHEELS) ? 0 : config.wheel + 1;
        wheelChanged = true;
        buttons->resetButton(BUTTON_NEXT);
    }
    
    // Handle PREV button for previous wheel pattern
    if (buttons->isPressed(BUTTON_PREV)) {
        config.wheel = (config.wheel == 0) ? MAX_WHEELS - 1 : config.wheel - 1;
        wheelChanged = true;
        buttons->resetButton(BUTTON_PREV);
    }
    
    if (wheelChanged) {
        // Force immediate display update
        lcdManager->forceRefresh();
    }
}

void UIController::handleRPMAdjustment() {
    if (currentState != UI_STATE_NORMAL) {
        return;
    }
    
    // Skip RPM adjustment in POT mode (buttons are for wheel selection)
    if (config.mode == POT_RPM) {
        return;
    }
    
    bool rpmChanged = false;
    uint16_t targetRPM = 0;
    
    // Determine target RPM based on current mode
    switch (config.mode) {
        case FIXED_RPM:
            targetRPM = config.fixed_rpm;
            break;
        case LINEAR_SWEPT_RPM:
            // In sweep mode, adjust the base RPM (could be sweep center point)
            targetRPM = currentStatus.base_rpm;
            break;
        default:
            targetRPM = currentStatus.rpm;
            break;
    }
    
    // Handle RPM increase (supports both single press and hold-to-repeat)
    if (buttons->shouldIncreaseRPM()) {
        int32_t newRPM = (int32_t)targetRPM + RPM_INCREMENT;
        if (newRPM > RPM_MAX) newRPM = RPM_MAX;
        targetRPM = (uint16_t)newRPM;
        rpmChanged = true;
    }
    
    // Handle RPM decrease (supports both single press and hold-to-repeat)
    if (buttons->shouldDecreaseRPM()) {
        int32_t newRPM = (int32_t)targetRPM - RPM_INCREMENT;
        if (newRPM < RPM_MIN) newRPM = RPM_MIN;
        targetRPM = (uint16_t)newRPM;
        rpmChanged = true;
    }
    
    if (rpmChanged) {
        // Update the appropriate RPM value based on mode
        switch (config.mode) {
            case FIXED_RPM:
                config.fixed_rpm = targetRPM;
                break;
            case LINEAR_SWEPT_RPM:
                currentStatus.base_rpm = targetRPM;
                break;
            default:
                currentStatus.rpm = targetRPM;
                break;
        }
        
        // Force immediate display update
        lcdManager->forceRefresh();
    }
}

void UIController::handleModeChange() {
    if (currentState != UI_STATE_NORMAL) {
        return;
    }
    
    if (buttons->isPressed(BUTTON_HELP)) {
        config.mode = (config.mode + 1 >= MAX_MODES) ? 0 : config.mode + 1;
        buttons->resetButton(BUTTON_HELP);
        
        // Force immediate display update
        lcdManager->forceRefresh();
    }
}

void UIController::handleSave() {
    if (currentState != UI_STATE_NORMAL) {
        return;
    }
    
    if (buttons->isPressed(BUTTON_SAVE)) {
        // Enter saving state
        currentState = UI_STATE_SAVING;
        stateTimeout = millis() + 2000; // 2 second timeout
        
        // Show saving message - now can use longer message with 20x4 display
        lcdManager->showMessage("SAVING...", MESSAGE_TIMEOUT_SHORT);
        
        // Perform the save operation
        // Note: saveConfig() doesn't return error status in current implementation
        // but we could enhance it in the future
        saveConfig();
        
        // For now, always show success since saveConfig() doesn't report errors
        // In a future enhancement, we could check EEPROM status
        lcdManager->showMessage("SAVED", MESSAGE_TIMEOUT_SHORT);
        
        buttons->resetButton(BUTTON_SAVE);
    }
}



bool UIController::hasSystemStateChanged() {
    return (lastWheel != config.wheel ||
            lastRPM != currentStatus.rpm ||
            lastMode != config.mode);
}

void UIController::updateCachedState() {
    lastWheel = config.wheel;
    lastRPM = currentStatus.rpm;
    lastMode = config.mode;
}

// Removed helper functions - inlined in calling code to save flash memory

bool UIController::isSafeToModifyState() {
    // Always safe to modify state in this implementation
    // Could be extended to check for critical timing conditions
    return buttons->isSafeToProcessActions();
}

UIState UIController::getCurrentState() {
    return currentState;
}

void UIController::returnToNormal() {
    currentState = UI_STATE_NORMAL;
    stateTimeout = 0;
    lcdManager->returnToMain();
}

bool UIController::isBusy() {
    return (currentState != UI_STATE_NORMAL);
}

void UIController::emergencyStop() {
    currentState = UI_STATE_NORMAL;
    stateTimeout = 0;
    initialized = false;
}

