# Implementation Plan

- [x] 1. Set up project dependencies and basic infrastructure

  - Add LiquidCrystal_I2C library dependency to platformio.ini
  - Create header files for LCD interface modules
  - Define pin configurations and constants for button inputs
  - _Requirements: 5.1, 5.3_

- [x] 2. Implement display abstraction layer

  - [x] 2.1 Create DisplayInterface abstract base class

    - Define virtual methods for hardware-independent display operations

    - Include init(), clear(), setCursor(), print() methods
    - _Requirements: 5.5_

  - [x] 2.2 Implement LCDDisplay concrete class for I2C LCD

    - Create LCDDisplay class inheriting from DisplayInterface
    - Implement I2C LCD initialization with error handling
    - Add graceful fallback when LCD hardware is not available
    - _Requirements: 1.5, 5.1_

- [x] 3. Create button input management system





  - [x] 3.1 Implement ButtonManager class with debouncing



    - Create non-blocking debounce algorithm using millis()
    - Define button state structure and tracking arrays
    - Implement button pin initialization and input reading
    - _Requirements: 6.1, 6.3, 6.4_



  - [x] 3.2 Add button repeat functionality for held buttons
    - Implement repeat delay and repeat rate timing
    - Add logic for RPM adjustment when NEXT/PREV buttons are held
    - Ensure repeat actions don't interfere with pattern generation
    - _Requirements: 3.2, 3.3, 6.2, 6.5_

- [x] 4. Develop LCD display management system





  - [x] 4.1 Create LCDManager class for display coordination



    - Implement display mode enumeration (MAIN, STATUS, MESSAGE)
    - Create state change detection using cached values
    - Add message timeout handling for temporary displays
    - _Requirements: 1.1, 1.2, 1.3, 1.4_
    


  - [x] 4.2 Implement main display screen with Bahasa Indonesia text

    - Create main screen layout showing wheel name, RPM, and mode
    - Use Bahasa Indonesia labels: "Pola:", "RPM:", "Mode:"
    - Keep wheel pattern names in original English
    - Add RPM and mode value formatting functions

    - _Requirements: 1.1, 7.1_

  - [x] 4.3 Add status display screen functionality


    - Implement status screen showing memory usage, uptime, version
    - Use Bahasa Indonesia labels: "Memori:", "Waktu:", "Versi:"
    - Add free RAM calculation and display formatting
    - Include system uptime calculation in minutes/hours
    - _Requirements: 4.4, 7.5_

  - [x] 4.4 Create message display system with Indonesian text


    - Implement temporary message display with timeout
    - Add Indonesian messages: "TERSIMPAN" (saved), "ERROR", "MENYIMPAN..." (saving)
    - Create message queue system for multiple messages
    - _Requirements: 4.2, 4.3_

- [x] 5. Implement user interface controller






  - [x] 5.1 Create UIController class for button action coordination



    - Initialize button manager and LCD manager integration
    - Implement main update loop for UI processing
    - Add state machine for different UI modes
    - _Requirements: 2.1, 2.2, 3.1_


  - [x] 5.2 Add wheel pattern selection functionality


    - Implement NEXT button handler for next wheel pattern
    - Implement PREV button handler for previous wheel pattern
    - Add wheel pattern wraparound logic (first/last)
    - Update display immediately when pattern changes
    - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5_

  - [x] 5.3 Implement RPM control functionality



    - Add HELP button handler for cycling through RPM modes
    - Implement RPM adjustment when NEXT/PREV buttons are held
    - Add RPM increment/decrement logic with bounds checking
    - Display mode changes with Indonesian text: "TETAP" (fixed), "POT", "SWEEP"
    - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5_

  - [x] 5.4 Add configuration save functionality



    - Implement SAVE button handler for EEPROM storage
    - Add save confirmation with "MENYIMPAN..." message
    - Display "TERSIMPAN" success message or "ERROR SIMPAN" on failure
    - Integrate with existing saveConfig() function
    - _Requirements: 4.1, 4.2, 4.3_


  - [x] 5.5 Implement status display functionality

    - Add ABT button handler for status information display
    - Show status screen for 3 seconds with automatic return
    - Display memory usage, uptime, and firmware version
    - Use Indonesian labels for all status information
    - _Requirements: 4.4, 7.5_

- [x] 6. Integrate LCD interface with main firmware






  - [x] 6.1 Add LCD initialization to setup() function



    - Initialize I2C communication and LCD hardware
    - Create display and button manager instances
    - Add error handling for LCD initialization failure
    - _Requirements: 1.5, 5.2_

  - [x] 6.2 Integrate UI updates in main loop



    - Add UI controller update calls to main loop()
    - Ensure LCD operations don't block pattern generation
    - Implement non-blocking display refresh logic
    - _Requirements: 5.3, 5.4, 6.5_


  - [x] 6.3 Add compression status display support

    - Show compression simulation status on LCD when active
    - Display compression modifier value with Indonesian label "Kompresi:"
    - Update compression display when values change
    - _Requirements: 7.2_

- [ ] 7. Implement error handling and recovery

  - [ ] 7.1 Add I2C communication error handling

    - Implement timeout detection for I2C operations
    - Add automatic retry logic for failed communications
    - Gracefully degrade to serial-only mode on persistent failures
    - _Requirements: 5.2_

  - [ ] 7.2 Create button hardware failure detection
    - Monitor button responsiveness and detect failures
    - Continue operation with remaining functional buttons
    - Log button failures to serial output
    - _Requirements: 6.6_

- [ ] 8. Add memory optimization and validation

  - [ ] 8.1 Optimize memory usage for Arduino Nano constraints

    - Use PROGMEM for Indonesian text strings to save RAM
    - Minimize dynamic memory allocation
    - Optimize string operations and buffer usage
    - _Requirements: 5.1_

  - [ ] 8.2 Add compile-time memory validation
    - Create build checks for flash memory usage limits
    - Add warnings when approaching memory constraints
    - Validate target memory footprint (<4KB flash, <200 bytes RAM)
    - _Requirements: 5.1_

- [ ] 9. Create comprehensive testing framework

  - [ ] 9.1 Implement unit tests for button debouncing

    - Test debounce timing accuracy with simulated inputs
    - Verify repeat rate functionality for held buttons
    - Validate simultaneous button press handling
    - _Requirements: 6.1, 6.2, 6.3_

  - [ ] 9.2 Add display update testing

    - Test LCD refresh triggers on state changes
    - Verify message timeout behavior
    - Validate screen transition logic between modes
    - _Requirements: 1.1, 1.2, 1.3, 1.4_

  - [ ] 9.3 Create integration tests for non-interference
    - Verify pattern generation accuracy is maintained
    - Test timer interrupt timing remains unaffected
    - Confirm serial communication continues to work
    - _Requirements: 5.3, 5.4_

- [ ] 10. Final integration and system testing

  - [ ] 10.1 Perform complete system integration testing

    - Test all button functions with LCD display updates
    - Verify Indonesian text displays correctly on LCD
    - Confirm wheel names remain in original English
    - Test error recovery and graceful degradation
    - _Requirements: All requirements_

  - [ ] 10.2 Validate memory usage and performance
    - Measure actual flash and RAM usage
    - Confirm performance benchmarks are met
    - Test system stability under continuous operation
    - Verify no memory leaks or stack overflow issues
    - _Requirements: 5.1, 5.2_
