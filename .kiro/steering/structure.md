# Project Structure & Architecture

## Directory Organization
```
ardustim/                   # Main source directory (PlatformIO src_dir)
├── ardustim.ino           # Main application entry point
├── ardustim.h             # Core function prototypes
├── globals.h              # Global structures and constants
├── enums.h                # System enumerations
├── wheel_defs.h           # Wheel pattern definitions
├── pin_config.h           # Hardware pin assignments
├── comms.cpp/h            # Serial communication interface
├── storage.ino/h          # EEPROM configuration management
└── LCD Interface Module:
    ├── display_interface.h    # Hardware abstraction layer
    ├── lcd_display.cpp/h      # Concrete LCD implementation
    ├── lcd_manager.cpp/h      # Display coordination
    ├── button_manager.cpp/h   # Button input handling
    └── ui_controller.cpp/h    # UI state machine

include/                    # PlatformIO include directory
lib/                       # PlatformIO library directory
test/                      # Unit tests
.pio/                      # PlatformIO build artifacts
```

## Architecture Patterns

### Layered Architecture
- **Hardware Abstraction**: `DisplayInterface` class for display independence
- **Manager Layer**: Coordinate hardware components (`LCDManager`, `ButtonManager`)
- **Controller Layer**: Business logic and state management (`UIController`)
- **Application Layer**: Main loop and interrupt handlers

### Memory Management
- **PROGMEM**: Wheel patterns stored in flash memory
- **Volatile**: ISR-accessed variables marked volatile
- **Packed Structs**: Configuration structures use `__attribute__((packed))`

### Code Style Conventions
- **File Headers**: Vim modeline + GPL license header on all files
- **Indentation**: 2 spaces, no tabs (`expandtab sw=2 softtabstop=2`)
- **Naming**: 
  - Classes: PascalCase (`LCDManager`)
  - Functions: camelCase (`updateDisplay`)
  - Constants: UPPER_SNAKE_CASE (`PIN_BUTTON_NEXT`)
  - Variables: camelCase (`currentStatus`)
- **Comments**: Doxygen-style for public APIs

### Conditional Compilation
- `#if ENABLE_LCD_INTERFACE` - LCD functionality can be disabled
- Platform-specific code for ATmega1280/2560 support
- Memory-constrained optimizations

### Interrupt-Driven Design
- Timer1: Pattern generation (critical timing)
- Timer2: Sweep functionality
- ADC: Potentiometer reading
- All ISR variables marked `volatile`

### Configuration Management
- EEPROM persistence for user settings
- Default values in `globals.h`
- Version-aware configuration loading