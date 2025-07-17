/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Button Input Manager
 * 
 * Handles button debouncing, repeat actions, and state management
 * Non-blocking implementation for Arduino Nano
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __BUTTON_MANAGER_H__
#define __BUTTON_MANAGER_H__

#include <stdint.h>
#include <Arduino.h>

/**
 * Button Pin Configuration
 * Based on design requirements for Arduino Nano
 */
#define BUTTON_PREV_PIN     2   // D2 - Previous wheel pattern / RPM decrease
#define BUTTON_NEXT_PIN     3   // D3 - Next wheel pattern / RPM increase  
#define BUTTON_SAVE_PIN     4   // D4 - Save configuration to EEPROM
#define BUTTON_ABT_PIN      5   // D5 - Show status information
#define BUTTON_HELP_PIN     6   // D6 - Cycle through RPM modes

/**
 * Button Enumeration for array indexing
 */
enum ButtonIndex {
    BUTTON_PREV = 0,
    BUTTON_NEXT = 1,
    BUTTON_SAVE = 2,
    BUTTON_ABT = 3,
    BUTTON_HELP = 4,
    BUTTON_COUNT = 5
};

/**
 * Button Timing Configuration
 */
#define BUTTON_DEBOUNCE_TIME    100     // 200ms debounce period (much longer to prevent bouncing)
#define BUTTON_REPEAT_DELAY     800     // 800ms before repeat starts (reasonable delay)
#define BUTTON_REPEAT_RATE      150     // 150ms between repeats (fast repeat for holding)

/**
 * Button debounce states
 */
enum ButtonDebounceState {
    BUTTON_IDLE,
    BUTTON_PRESSED_DEBOUNCE,
    BUTTON_PRESSED_STABLE,
    BUTTON_RELEASED_DEBOUNCE
};

/**
 * Button state structure for tracking individual button states
 */
struct ButtonState {
    ButtonDebounceState debounceState;  // Current debounce state
    bool currentReading;                // Current raw pin reading
    bool stableState;                   // Stable debounced state
    bool pressedThisLoop;               // True if button was just pressed this loop
    uint32_t stateChangeTime;           // Time when state last changed
    uint32_t pressTime;                 // Time when button was first pressed
    uint32_t lastRepeat;                // Time of last repeat action
};

/**
 * Button Manager Class
 * Handles all button input processing with non-blocking algorithms
 */
class ButtonManager {
private:
    ButtonState buttons[BUTTON_COUNT];
    static const uint8_t BUTTON_PINS[BUTTON_COUNT];
    
    /**
     * Read raw button state from pin
     * @param buttonIndex Index of button to read
     * @return true if button is physically pressed
     */
    bool readButtonPin(uint8_t buttonIndex);
    
    /**
     * Update debounce state for a single button
     * @param buttonIndex Index of button to update
     */
    void updateButtonDebounce(uint8_t buttonIndex);
    
    /**
     * Update repeat state for a single button
     * @param buttonIndex Index of button to update
     */
    void updateButtonRepeat(uint8_t buttonIndex);
    
public:
    /**
     * Constructor - initializes button states
     */
    ButtonManager();
    
    /**
     * Initialize button pins and internal state
     * Call once during setup()
     */
    void init();
    
    /**
     * Update all button states - call regularly in main loop
     * Non-blocking operation using millis() timing
     */
    void update();
    
    /**
     * Check if button was just pressed (single press detection)
     * @param buttonIndex Button to check
     * @return true if button was just pressed
     */
    bool isPressed(uint8_t buttonIndex);
    
    /**
     * Check if button is currently being held down
     * @param buttonIndex Button to check
     * @return true if button is held
     */
    bool isHeld(uint8_t buttonIndex);
    
    /**
     * Check if button is in repeat mode (held and repeating)
     * @param buttonIndex Button to check
     * @return true if button should trigger repeat action
     */
    bool isRepeating(uint8_t buttonIndex);
    
    /**
     * Reset button state (useful after processing an action)
     * @param buttonIndex Button to reset
     */
    void resetButton(uint8_t buttonIndex);
    
    /**
     * Check if NEXT button should trigger RPM increase
     * Handles both single press and repeat for held button
     * @return true if RPM should be increased
     */
    bool shouldIncreaseRPM();
    
    /**
     * Check if PREV button should trigger RPM decrease
     * Handles both single press and repeat for held button
     * @return true if RPM should be decreased
     */
    bool shouldDecreaseRPM();
    
    /**
     * Check if any button that affects RPM is currently held
     * Used to determine if we're in RPM adjustment mode
     * @return true if NEXT or PREV button is held
     */
    bool isRPMAdjustmentActive();
    
    /**
     * Check if it's safe to process button actions
     * Ensures button processing doesn't interfere with critical timing
     * @return true if button actions can be safely processed
     */
    bool isSafeToProcessActions();
};

#endif