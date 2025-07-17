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
                           forceStaticLabelReset(false),
                           lastWheel(255), lastRPM(0), lastMode(255), 
                           isDisplayInitialized(false), messageModeInitialized(false) {
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
    isDisplayInitialized = false;
    messageModeInitialized = false;
    
    messageBuffer[0] = '\0';
}

void LCDManager::update() {
    if (!display || !display->isAvailable()) {
        return;
    }
    
    uint32_t currentTime = millis();
    
    // Handle message timeout (but not during startup)
    if (currentMode == DISPLAY_MESSAGE && messageTimeout > 0 && currentTime >= messageTimeout) {
        returnToMain();  // Use returnToMain() to properly reset all flags
    }
    
    // Skip main display updates during startup mode
    if (currentMode == DISPLAY_STARTUP) {
        return;
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
            case DISPLAY_STARTUP:
                // Startup display is handled externally by handleStartupSequence()
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
    
    // Use smaller thresholds for more responsive display updates
    // But still filter out minor fluctuations
    uint16_t rpmThreshold = 100;
    if (config.mode == POT_RPM) {
        rpmThreshold = 200; // Moderate threshold for POT mode with improved ADC filtering
    } else if (config.mode == LINEAR_SWEPT_RPM) {
        rpmThreshold = 100; // Smaller threshold for sweep mode to see smooth changes
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
    
    char buffer[21];  // 20 chars + null terminator for 20x4 display
    static uint8_t lastWheelDisplayed = 255;
    static uint16_t lastRPMDisplayed = 0;
    static uint8_t lastModeDisplayed = 255;
    static bool staticLabelsInitialized = false;
    
    // Initialize static labels only once (NEVER use clear() in loop)
    if (!staticLabelsInitialized || forceStaticLabelReset) {
        display->clear();
        
        // Initialize Line 2: RPM label (static - write once)
        display->setCursor(0, 1);
        display->print("RPM: ");
        
        // Initialize Line 3: Mode label (static - write once)
        display->setCursor(0, 2);
        display->print("Mode: ");
        
        // Initialize Line 4: Status (static - write once)
        display->setCursor(0, 3);
        display->print("Ready");
        
        staticLabelsInitialized = true;
        isDisplayInitialized = true;
        forceStaticLabelReset = false;  // Reset the flag
        
        // Force all content to be displayed after initialization
        lastWheelDisplayed = 255;
        lastRPMDisplayed = 0;
        lastModeDisplayed = 255;
    }
    
    // Update Line 1: Wheel pattern name (only if changed)
    if (config.wheel != lastWheelDisplayed) {
        display->setCursor(0, 0);
        // Use spaces to overwrite old content (Arduino community best practice)
        display->print("                    "); // 20 spaces to clear line
        display->setCursor(0, 0);
        getWheelName(config.wheel, buffer, sizeof(buffer));
        display->print(buffer);
        lastWheelDisplayed = config.wheel;
    }
    
    // Update Line 2: RPM number only (preserve "RPM: " label)
    if (currentStatus.rpm != lastRPMDisplayed) {
        display->setCursor(5, 1);
        // Clear only the RPM number area with spaces
        display->print("               "); // 15 spaces (20 total - 5 for "RPM: ")
        display->setCursor(5, 1);
        
        formatRPM(currentStatus.rpm, buffer, sizeof(buffer));
        display->print(buffer);
        lastRPMDisplayed = currentStatus.rpm;
    }
    
    // Update Line 3: Mode text only (preserve "Mode: " label)
    if (config.mode != lastModeDisplayed) {
        display->setCursor(6, 2);
        // Clear only the mode text area with spaces
        display->print("              "); // 14 spaces (20 total - 6 for "Mode: ")
        display->setCursor(6, 2);
        
        formatMode(config.mode, buffer, sizeof(buffer));
        display->print(buffer);
        lastModeDisplayed = config.mode;
    }
}



void LCDManager::updateMessageDisplay() {
    if (!display) return;
    
    // Only clear when switching to message mode (not every update)
    if (!messageModeInitialized) {
        display->clear();
        messageModeInitialized = true;
    }
    
    // Center the message on line 2 (middle of 20x4 display)
    uint8_t messageLen = strlen(messageBuffer);
    uint8_t startPos = (20 - messageLen) / 2;
    
    // Clear the message line with spaces first
    display->setCursor(0, 1);
    display->print("                    "); // 20 spaces
    
    // Then write the message
    display->setCursor(startPos, 1);
    display->print(messageBuffer);
}

void LCDManager::returnToMain() {
    currentMode = DISPLAY_MAIN;
    messageTimeout = 0;
    needsRefresh = true;
    
    // Reset message mode flag so it can be re-initialized next time
    messageModeInitialized = false;
    
    // Force static labels to be reinitialized since message mode cleared the display
    forceStaticLabelReset = true;
}

void LCDManager::formatRPM(uint16_t rpm, char* buffer, uint8_t bufferSize) {
    // Always show full RPM numbers, no "k" suffix
    snprintf(buffer, bufferSize, "%d", rpm);
}

void LCDManager::formatMode(uint8_t mode, char* buffer, uint8_t bufferSize) {
    switch (mode) {
        case FIXED_RPM:
            strncpy(buffer, "Fixed", bufferSize - 1);
            break;
        case POT_RPM:
            strncpy(buffer, "Pot Control", bufferSize - 1);
            break;
        case LINEAR_SWEPT_RPM:
            strncpy(buffer, "Linear Sweep", bufferSize - 1);
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
    
    // Get the wheel name from PROGMEM
    const char* wheelName = nullptr;
    switch (wheelIndex) {
        case DIZZY_FOUR_CYLINDER:
            wheelName = dizzy_four_cylinder_friendly_name;
            break;
        case DIZZY_SIX_CYLINDER:
            wheelName = dizzy_six_cylinder_friendly_name;
            break;
        case DIZZY_EIGHT_CYLINDER:
            wheelName = dizzy_eight_cylinder_friendly_name;
            break;
        case SIXTY_MINUS_TWO:
            wheelName = sixty_minus_two_friendly_name;
            break;
        case SIXTY_MINUS_TWO_WITH_CAM:
            wheelName = sixty_minus_two_with_cam_friendly_name;
            break;
        case SIXTY_MINUS_TWO_WITH_HALFMOON_CAM:
            wheelName = sixty_minus_two_with_halfmoon_cam_friendly_name;
            break;
        case THIRTY_SIX_MINUS_ONE:
            wheelName = thirty_six_minus_one_friendly_name;
            break;
        case TWENTY_FOUR_MINUS_ONE:
            wheelName = twenty_four_minus_one_friendly_name;
            break;
        case FOUR_MINUS_ONE_WITH_CAM:
            wheelName = four_minus_one_with_cam_friendly_name;
            break;
        case EIGHT_MINUS_ONE:
            wheelName = eight_minus_one_friendly_name;
            break;
        case SIX_MINUS_ONE_WITH_CAM:
            wheelName = six_minus_one_with_cam_friendly_name;
            break;
        case TWELVE_MINUS_ONE_WITH_CAM:
            wheelName = twelve_minus_one_with_cam_friendly_name;
            break;
        case FOURTY_MINUS_ONE:
            wheelName = fourty_minus_one_friendly_name;
            break;
        case DIZZY_FOUR_TRIGGER_RETURN:
            wheelName = dizzy_four_trigger_return_friendly_name;
            break;
        case ODDFIRE_VR:
            wheelName = oddfire_vr_friendly_name;
            break;
        case OPTISPARK_LT1:
            wheelName = optispark_lt1_friendly_name;
            break;
        case TWELVE_MINUS_THREE:
            wheelName = twelve_minus_three_friendly_name;
            break;
        case THIRTY_SIX_MINUS_TWO_TWO_TWO:
            wheelName = thirty_six_minus_two_two_two_friendly_name;
            break;
        case THIRTY_SIX_MINUS_TWO_TWO_TWO_H6:
            wheelName = thirty_six_minus_two_two_two_h6_friendly_name;
            break;
        case THIRTY_SIX_MINUS_TWO_TWO_TWO_WITH_CAM:
            wheelName = thirty_six_minus_two_two_two_with_cam_friendly_name;
            break;
        case FOURTY_TWO_HUNDRED_WHEEL:
            wheelName = fourty_two_hundred_wheel_friendly_name;
            break;
        case THIRTY_SIX_MINUS_ONE_WITH_CAM_FE3:
            wheelName = thirty_six_minus_one_with_cam_fe3_friendly_name;
            break;
        case SIX_G_SEVENTY_TWO_WITH_CAM:
            wheelName = six_g_seventy_two_with_cam_friendly_name;
            break;
        case BUELL_ODDFIRE_CAM:
            wheelName = buell_oddfire_cam_friendly_name;
            break;
        case GM_LS1_CRANK_AND_CAM:
            wheelName = gm_ls1_crank_and_cam_friendly_name;
            break;
        case GM_58x_LS_CRANK_4X_CAM:
            wheelName = gm_ls_58X_crank_and_4x_cam_friendly_name;
            break;
        case LOTUS_THIRTY_SIX_MINUS_ONE_ONE_ONE_ONE:
            wheelName = lotus_thirty_six_minus_one_one_one_one_friendly_name;
            break;
        case HONDA_RC51_WITH_CAM:
            wheelName = honda_rc51_with_cam_friendly_name;
            break;
        case THIRTY_SIX_MINUS_ONE_WITH_SECOND_TRIGGER:
            wheelName = thirty_six_minus_one_with_second_trigger_friendly_name;
            break;
        case WEBER_IAW_WITH_CAM:
            wheelName = weber_iaw_with_cam_friendly_name;
            break;
        case FIAT_ONE_POINT_EIGHT_SIXTEEN_VALVE_WITH_CAM:
            wheelName = fiat_one_point_eight_sixteen_valve_with_cam_friendly_name;
            break;
        case THREE_SIXTY_NISSAN_CAS:
            wheelName = three_sixty_nissan_cas_friendly_name;
            break;
        case TWENTY_FOUR_MINUS_TWO_WITH_SECOND_TRIGGER:
            wheelName = twenty_four_minus_two_with_second_trigger_friendly_name;
            break;
        case YAMAHA_EIGHT_TOOTH_WITH_CAM:
            wheelName = yamaha_eight_tooth_with_cam_friendly_name;
            break;
        case MITSUBISH_4g63_4_2:
            wheelName = mitsubishi_4g63_4_2_friendly_name;
            break;
        case AUDI_135_WITH_CAM:
            wheelName = audi_135_with_cam_friendly_name;
            break;
        case HONDA_D17_NO_CAM:
            wheelName = honda_d17_no_cam_friendly_name;
            break;
        case MAZDA_323_AU:
            wheelName = mazda_323_au_friendly_name;
            break;
        case DAIHATSU_3CYL:
            wheelName = daihatsu_3cyl_friendly_name;
            break;
        case MIATA_9905:
            wheelName = miata_9905_friendly_name;
            break;
        case TWELVE_WITH_CAM:
            wheelName = twelve_with_cam_friendly_name;
            break;
        case TWENTY_FOUR_WITH_CAM:
            wheelName = twenty_four_with_cam_friendly_name;
            break;
        case SUBARU_SIX_SEVEN:
            wheelName = subaru_six_seven_name_friendly_name;
            break;
        case GM_7X:
            wheelName = gm_seven_x_friendly_name;
            break;
        case FOUR_TWENTY_A:
            wheelName = four_twenty_a_friendly_name;
            break;
        case FORD_ST170:
            wheelName = ford_st170_friendly_name;
            break;
        case MITSUBISHI_3A92:
            wheelName = mitsubishi_3A92_friendly_name;
            break;
        case TOYOTA_4AGE_CAS:
            wheelName = Toyota_4AGE_CAS_friendly_name;
            break;
        case TOYOTA_4AGZE:
            wheelName = Toyota_4AGZE_friendly_name;
            break;
        case SUZUKI_DRZ400:
            wheelName = Suzuki_DRZ400_friendly_name;
            break;
        case JEEP2000_4CYL:
            wheelName = Jeep_2000_4cyl_friendly_name;
            break;
        case VIPER_96_02:
            wheelName = VIPER9602_friendly_name;
            break;
        case THIRTY_SIX_MINUS_TWO_WITH_ONE_CAM:
            wheelName = thirty_six_minus_two_with_second_trigger_friendly_name;
            break;
        case GM_40_OSS:
            wheelName = GM_40_Tooth_Trans_OSS_friendly_name;
            break;
        default:
            strncpy(buffer, "Unknown", bufferSize - 1);
            buffer[bufferSize - 1] = '\0';
            return;
    }
    
    // Copy string from PROGMEM
    strncpy_P(buffer, wheelName, bufferSize - 1);
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
    
    // Reset message mode flag to ensure display gets cleared
    messageModeInitialized = false;
}

void LCDManager::enterStartupMode() {
    if (!display) return;
    
    currentMode = DISPLAY_STARTUP;
    messageTimeout = 0;  // No timeout during startup
    needsRefresh = true;
    
    // Clear display for startup sequence
    display->clear();
}

void LCDManager::showStartupMessage(const char* message) {
    if (!display || !message || currentMode != DISPLAY_STARTUP) return;
    
    // Clear display and show startup message
    display->clear();
    
    // Center the message on line 1 (top line for startup)
    uint8_t messageLen = strlen(message);
    uint8_t startPos = (20 - messageLen) / 2;
    
    display->setCursor(startPos, 1);
    display->print(message);
    
    needsRefresh = true;
}

void LCDManager::exitStartupMode() {
    if (currentMode == DISPLAY_STARTUP) {
        returnToMain();
    }
}

void LCDManager::forceRefresh() {
    forceRefreshFlag = true;
    needsRefresh = true;
    // Note: We don't reset static initialization to avoid clearing static labels
    // The static labels ("RPM:", "Mode:", "Ready") remain persistent
}

bool LCDManager::isDisplayAvailable() {
    return display && display->isAvailable();
}