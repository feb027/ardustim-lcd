# Technology Stack & Build System

## Build System
- **PlatformIO** - Primary build system and dependency management
- **Arduino Framework** - Core embedded framework
- **AVR GCC** - Compiler toolchain for ATmega328P

## Target Platforms
- `diecimilaatmega328` - Arduino Duemilanove
- `nanoatmega328` - Arduino Nano (primary target)
- `uno` - Arduino Uno

## Key Libraries
- `LiquidCrystal_I2C@^1.1.4` - I2C LCD display driver
- `EEPROM` - Configuration persistence
- `avr/pgmspace.h` - Flash memory storage for wheel patterns

## Build Configuration
- Source directory: `ardustim/`
- Monitor speed: 115200 baud
- Optimization flags for Nano build:
  - `-Os` - Size optimization
  - `-flto` - Link-time optimization
  - `-ffunction-sections -fdata-sections` - Dead code elimination
  - `-Wl,--gc-sections` - Linker garbage collection

## Common Commands
```bash
# Build for Arduino Nano
pio run -e nano

# Upload to device
pio run -e nano -t upload

# Monitor serial output
pio device monitor

# Clean build
pio run -t clean

# Build all environments
pio run
```

## Memory Constraints
- Flash: 32KB (ATmega328P)
- RAM: 2KB (ATmega328P)
- EEPROM: 1KB for configuration storage

## Compilation Flags
- `ENABLE_LCD_INTERFACE=1` - Enable LCD interface (Nano build)
- Size optimization prioritized for resource-constrained hardware