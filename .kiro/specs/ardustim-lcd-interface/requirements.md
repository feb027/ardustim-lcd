# Requirements Document

## Introduction

This feature adds an LCD display interface with button controls to the existing Ardu-Stim ECU simulator firmware. The enhancement will provide local control and real-time status display without requiring a PC connection, while maintaining all existing serial communication functionality. The implementation must be memory-efficient for Arduino Nano's 32KB flash constraint and designed for future scalability to larger TFT displays.

## Requirements

### Requirement 1

**User Story:** As an ECU technician, I want to see the current wheel pattern, RPM, and mode on an LCD display, so that I can monitor the simulator status without connecting to a PC.

#### Acceptance Criteria

1. WHEN the system starts THEN the LCD SHALL display the current wheel pattern name, RPM value, and operating mode
2. WHEN the RPM changes THEN the LCD SHALL update the RPM display within 100ms
3. WHEN the wheel pattern changes THEN the LCD SHALL update the pattern name display immediately
4. WHEN the operating mode changes THEN the LCD SHALL update the mode display immediately
5. IF the LCD initialization fails THEN the system SHALL continue operating normally with serial interface only

### Requirement 2

**User Story:** As an ECU technician, I want to navigate through different wheel patterns using physical buttons, so that I can quickly change patterns without using serial commands.

#### Acceptance Criteria

1. WHEN the NEXT button (D3) is pressed THEN the system SHALL select the next wheel pattern in the list
2. WHEN the PREV button (D2) is pressed THEN the system SHALL select the previous wheel pattern in the list
3. WHEN at the last wheel pattern and NEXT is pressed THEN the system SHALL wrap to the first pattern
4. WHEN at the first wheel pattern and PREV is pressed THEN the system SHALL wrap to the last pattern
5. WHEN a wheel pattern changes via button THEN the LCD SHALL display the new pattern name within 50ms

### Requirement 3

**User Story:** As an ECU technician, I want to adjust RPM settings using buttons, so that I can control the simulator speed without serial commands.

#### Acceptance Criteria

1. WHEN the HELP button (D6) is pressed THEN the system SHALL cycle through RPM control modes (FIXED, POT, SWEEP)
2. WHEN in FIXED mode and NEXT/PREV buttons are held THEN the RPM SHALL increase/decrease by 100 RPM increments
3. WHEN RPM adjustment reaches minimum (10 RPM) THEN further decrease SHALL be ignored
4. WHEN RPM adjustment reaches maximum (9000 RPM) THEN further increase SHALL be ignored
5. WHEN RPM changes via buttons THEN the LCD SHALL update the RPM display immediately

### Requirement 4

**User Story:** As an ECU technician, I want to save current settings to EEPROM using a button, so that my configuration persists after power cycling.

#### Acceptance Criteria

1. WHEN the SAVE button (D4) is pressed THEN the current configuration SHALL be saved to EEPROM
2. WHEN save operation completes successfully THEN the LCD SHALL display "SAVED" message for 1 second
3. WHEN save operation fails THEN the LCD SHALL display "SAVE ERROR" message for 2 seconds
4. WHEN the ABT button (D5) is pressed THEN the system SHALL display additional status information for 3 seconds

### Requirement 5

**User Story:** As a developer, I want the LCD interface to be memory efficient and modular, so that it doesn't impact the core timing functions and can be easily upgraded to TFT displays.

#### Acceptance Criteria

1. WHEN the LCD module is compiled THEN it SHALL use less than 4KB of flash memory
2. WHEN LCD operations execute THEN they SHALL NOT interfere with timer interrupts or pattern generation
3. WHEN the system runs THEN LCD updates SHALL occur only during main loop execution
4. WHEN button debouncing occurs THEN it SHALL use non-blocking algorithms
5. IF future TFT upgrade is needed THEN the display abstraction layer SHALL allow easy hardware substitution

### Requirement 6

**User Story:** As an ECU technician, I want button inputs to be reliable and responsive, so that I can efficiently control the simulator without accidental triggers.

#### Acceptance Criteria

1. WHEN any button is pressed THEN the system SHALL debounce the input for 50ms minimum
2. WHEN a button is held down THEN repeat actions SHALL begin after 500ms hold time
3. WHEN multiple buttons are pressed simultaneously THEN only the first detected button SHALL be processed
4. WHEN button interrupts occur THEN they SHALL NOT block critical timing operations
5. WHEN the system is generating patterns THEN button responsiveness SHALL remain consistent

### Requirement 7

**User Story:** As an ECU technician, I want the display to show comprehensive status information, so that I can verify all simulator parameters at a glance.

#### Acceptance Criteria

1. WHEN the main display is active THEN it SHALL show wheel name, current RPM, and mode on separate lines
2. WHEN compression simulation is active THEN the display SHALL indicate compression status and modifier value
3. WHEN in sweep mode THEN the display SHALL show current RPM and sweep range
4. WHEN using potentiometer control THEN the display SHALL indicate POT mode and current reading
5. WHEN the ABT button is pressed THEN the display SHALL show memory usage, uptime, and version information