/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Button Input Manager Implementation
 * 
 * Handles button debouncing, repeat actions, and state management
 * Non-blocking implementation for Arduino Nano
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */

#include "button_manager.h"
#include "pin_config.h"

// Static pin mapping array
const uint8_t ButtonManager::BUTTON_PINS[BUTTON_COUNT] = {
    PIN_BUTTON_PREV,    // BUTTON_PREV = 0
    PIN_BUTTON_NEXT,    // BUTTON_NEXT = 1  
    PIN_BUTTON_SAVE,    // BUTTON_SAVE = 2
    PIN_BUTTON_ABT,     // BUTTON_ABT = 3
    PIN_BUTTON_HELP     // BUTTON_HELP = 4
};

/**
 * Constructor - initializes button states to default values
 */
ButtonManager::ButtonManager() {
    // Initialize all button states
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].debounceState = BUTTON_IDLE;
        buttons[i].currentReading = HIGH;  // Buttons are active low
        buttons[i].stableState = false;
        buttons[i].pressedThisLoop = false;
        buttons[i].stateChangeTime = 0;
        buttons[i].pressTime = 0;
        buttons[i].lastRepeat = 0;
    }
}

/**
 * Initialize button pins and internal state
 * Call once during setup()
 */
void ButtonManager::init() {
    // Configure all button pins as inputs with internal pull-up resistors
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        
        // Initialize state machine
        buttons[i].debounceState = BUTTON_IDLE;
        buttons[i].currentReading = digitalRead(BUTTON_PINS[i]);
        buttons[i].stableState = false;
        buttons[i].pressedThisLoop = false;
        buttons[i].stateChangeTime = millis();
        buttons[i].pressTime = 0;
        buttons[i].lastRepeat = 0;
    }
}

/**
 * Read raw button state from pin
 * @param buttonIndex Index of button to read
 * @return true if button is physically pressed
 */
bool ButtonManager::readButtonPin(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return false;
    
    // Buttons are active low (pressed = LOW reading)
    return (digitalRead(BUTTON_PINS[buttonIndex]) == LOW);
}

/**
 * Update debounce state for a single button using proper state machine
 * @param buttonIndex Index of button to update
 */
void ButtonManager::updateButtonDebounce(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return;
    
    ButtonState* btn = &buttons[buttonIndex];
    uint32_t currentTime = millis();
    bool currentReading = readButtonPin(buttonIndex);
    
    // Reset pressed flag for this loop
    btn->pressedThisLoop = false;
    
    // State machine for proper debouncing
    switch (btn->debounceState) {
        case BUTTON_IDLE:
            if (currentReading) {  // Button pressed
                btn->debounceState = BUTTON_PRESSED_DEBOUNCE;
                btn->stateChangeTime = currentTime;
            }
            break;
            
        case BUTTON_PRESSED_DEBOUNCE:
            if (!currentReading) {  // Button released during debounce
                btn->debounceState = BUTTON_IDLE;
            } else if ((currentTime - btn->stateChangeTime) >= BUTTON_DEBOUNCE_TIME) {
                // Debounce period passed, button is stable pressed
                btn->debounceState = BUTTON_PRESSED_STABLE;
                btn->stableState = true;
                btn->pressedThisLoop = true;  // Signal a clean press
                btn->pressTime = currentTime;
            }
            break;
            
        case BUTTON_PRESSED_STABLE:
            if (!currentReading) {  // Button released
                btn->debounceState = BUTTON_RELEASED_DEBOUNCE;
                btn->stateChangeTime = currentTime;
            }
            break;
            
        case BUTTON_RELEASED_DEBOUNCE:
            if (currentReading) {  // Button pressed again during debounce
                btn->debounceState = BUTTON_PRESSED_STABLE;
            } else if ((currentTime - btn->stateChangeTime) >= BUTTON_DEBOUNCE_TIME) {
                // Debounce period passed, button is stable released
                btn->debounceState = BUTTON_IDLE;
                btn->stableState = false;
            }
            break;
    }
    
    btn->currentReading = currentReading;
}

/**
 * Update repeat state for a single button
 * @param buttonIndex Index of button to update
 */
void ButtonManager::updateButtonRepeat(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return;
    
    ButtonState* btn = &buttons[buttonIndex];
    uint32_t currentTime = millis();
    
    // Only process repeat for stable pressed buttons
    if (!btn->stableState) {
        btn->lastRepeat = 0;
        return;
    }
    
    // Check if button has been held long enough to start repeating
    if (btn->lastRepeat == 0 && (currentTime - btn->pressTime) >= BUTTON_REPEAT_DELAY) {
        btn->lastRepeat = currentTime;
    }
}

/**
 * Update all button states - call regularly in main loop
 * Non-blocking operation using millis() timing
 */
void ButtonManager::update() {
    // Update debounce and repeat state for all buttons
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        updateButtonDebounce(i);
        updateButtonRepeat(i);
    }
    
    // Simplified button handling - removed simultaneous press priority
}

/**
 * Check if button was just pressed (single press detection)
 * @param buttonIndex Button to check
 * @return true if button was just pressed
 */
bool ButtonManager::isPressed(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return false;
    
    ButtonState* btn = &buttons[buttonIndex];
    
    // Return true only if button was pressed this loop (clean single press)
    return btn->pressedThisLoop;
}

/**
 * Check if button is currently being held down
 * @param buttonIndex Button to check
 * @return true if button is held
 */
bool ButtonManager::isHeld(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return false;
    
    return buttons[buttonIndex].stableState;
}

/**
 * Check if button is in repeat mode (held and repeating)
 * @param buttonIndex Button to check
 * @return true if button should trigger repeat action
 */
bool ButtonManager::isRepeating(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return false;
    
    ButtonState* btn = &buttons[buttonIndex];
    uint32_t currentTime = millis();
    
    // Button must be held and repeat timer must have elapsed
    if (btn->stableState && btn->lastRepeat > 0 && (currentTime - btn->lastRepeat) >= BUTTON_REPEAT_RATE) {
        // Update last repeat time for next cycle
        btn->lastRepeat = currentTime;
        return true;
    }
    
    return false;
}

/**
 * Reset button state (useful after processing an action)
 * @param buttonIndex Button to reset
 */
void ButtonManager::resetButton(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return;
    
    ButtonState* btn = &buttons[buttonIndex];
    
    // Reset press detection but maintain physical state
    btn->pressedThisLoop = false;
    
    // If button is still physically pressed, maintain held state
    if (btn->stableState) {
        btn->pressTime = millis();
    }
}



/**
 * Check if NEXT button should trigger RPM increase
 * Handles both single press and repeat for held button
 * @return true if RPM should be increased
 */
bool ButtonManager::shouldIncreaseRPM() {
    // For wheel pattern selection: single press
    // For RPM adjustment: repeat when held
    return isPressed(BUTTON_NEXT) || isRepeating(BUTTON_NEXT);
}

/**
 * Check if PREV button should trigger RPM decrease
 * Handles both single press and repeat for held button
 * @return true if RPM should be decreased
 */
bool ButtonManager::shouldDecreaseRPM() {
    // For wheel pattern selection: single press
    // For RPM adjustment: repeat when held
    return isPressed(BUTTON_PREV) || isRepeating(BUTTON_PREV);
}

/**
 * Check if any button that affects RPM is currently held
 * Used to determine if we're in RPM adjustment mode
 * @return true if NEXT or PREV button is held
 */
bool ButtonManager::isRPMAdjustmentActive() {
    return isHeld(BUTTON_NEXT) || isHeld(BUTTON_PREV);
}

/**
 * Check if it's safe to process button actions
 * Ensures button processing doesn't interfere with critical timing
 * @return true if button actions can be safely processed
 */
bool ButtonManager::isSafeToProcessActions() {
    // Always safe to process - button operations are designed to be non-blocking
    // This method is provided for future expansion if timing constraints are added
    // All button processing uses millis() timing and avoids blocking operations
    return true;
}