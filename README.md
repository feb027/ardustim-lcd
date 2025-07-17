# ArduStim - Enhanced ECU Simulator with LCD Interface

A fork of the original ArduStim project, enhanced for Arduino Nano with 20x4 I2C LCD display support and optimized for 30KB flash memory constraints.

## Overview

ArduStim is an Arduino-based engine simulator that generates precise timing signals for testing automotive ECUs and engine management systems. This enhanced version adds a comprehensive LCD interface with button controls, allowing standalone operation without PC connection while maintaining all original serial communication functionality.

## Key Features

### Core Functionality
- **50+ Wheel Patterns**: Supports trigger wheels from major automotive manufacturers
- **Variable RPM Control**: 0-9000 RPM via potentiometer, fixed RPM, or sweep modes
- **Real-time Pattern Switching**: Change wheel patterns on-the-fly
- **Precision Timing**: Hardware timer-based signal generation for ECU testing accuracy
- **Serial Interface**: Full computer control via USB/serial commands

### Enhanced LCD Interface
- **20x4 I2C LCD Display**: Real-time status monitoring without PC connection
- **5-Button Control Panel**: Navigate patterns, adjust RPM, save settings
- **Multilingual Support**: Interface text in Bahasa Indonesia, wheel names in English
- **Non-blocking Operation**: LCD interface doesn't interfere with timing precision
- **Memory Optimized**: <4KB flash footprint for Arduino Nano compatibility

### Hardware Compatibility
- **Primary Target**: Arduino Nano (ATmega328P, 32KB flash)
- **Also Supports**: Arduino Uno, Duemilanove
- **Display**: 20x4 I2C LCD (address 0x27)
- **Memory Efficient**: Optimized for 30KB flash constraint

## Hardware Requirements

### Arduino Nano Setup
```
Arduino Nano (ATmega328P)
├── Digital Pins
│   ├── D2  → PREV Button (Previous pattern/RPM decrease)
│   ├── D3  → NEXT Button (Next pattern/RPM increase)
│   ├── D4  → SAVE Button (Save to EEPROM)
│   ├── D5  → ABT Button (Show status info)
│   ├── D6  → HELP Button (Cycle RPM modes)
│   ├── D8  → Primary Output (Crank signal)
│   ├── D9  → Secondary Output (Cam1 signal)
│   └── D10 → Tertiary Output (Cam2 signal)
├── Analog Pins
│   ├── A0  → RPM Potentiometer
│   ├── A4  → I2C SDA (LCD)
│   └── A5  → I2C SCL (LCD)
└── Power
    ├── 5V  → LCD VCC
    └── GND → Common Ground
```

### LCD Wiring (20x4 I2C)
```
LCD Module    Arduino Nano
VCC     →     5V
GND     →     GND
SDA     →     A4
SCL     →     A5
```

### Button Wiring
All buttons connect between digital pin and GND (active low):
```
Button   Pin   Function
PREV  →  D2  → Previous wheel pattern / RPM decrease (when held)
NEXT  →  D3  → Next wheel pattern / RPM increase (when held)
SAVE  →  D4  → Save current configuration to EEPROM
ABT   →  D5  → Show system status and information
HELP  →  D6  → Cycle through RPM control modes
```

## Software Features

### Build System
- **PlatformIO**: Modern build system with dependency management
- **Size Optimization**: Aggressive compiler flags for memory efficiency
- **Multi-target**: Support for Nano, Uno, and Duemilanove

### Memory Management
- **Flash Usage**: 29,646 bytes (96.5% of 30KB usable)
- **RAM Usage**: 1,795 bytes (87.6% of 2KB)
- **Highly Optimized**: Aggressive compiler optimizations for space
- **EEPROM**: Configuration persistence

### Control Modes
1. **Potentiometer Mode**: Real-time RPM control via analog input
2. **Fixed RPM Mode**: Set specific RPM value (10-9000 RPM)
3. **Sweep Mode**: Automatic RPM ramping between limits

## Installation

### Prerequisites
```bash
# Install PlatformIO
pip install platformio

# Or use PlatformIO IDE extension in VS Code
```

### Build and Upload
```bash
# Clone repository
git clone <repository-url>
cd ardustim-enhanced

# Build for Arduino Nano (with LCD interface)
pio run -e nano

# Upload to device
pio run -e nano -t upload

# Monitor serial output
pio device monitor
```

### Build Configurations
```ini
# Arduino Nano (primary target)
[env:nano]
build_flags = 
    -DENABLE_LCD_INTERFACE=1    # Enable LCD interface
    -Os                         # Size optimization
    -flto                       # Link-time optimization

# Arduino Uno (basic version)
[env:uno]
# Standard build without LCD interface

# Duemilanove (legacy support)
[env:diecimilaatmega328]
# Compatible with older Arduino boards
```

## Usage

### LCD Interface Operation

#### Main Display
```
Pola: 60-2 Tooth Wheel
RPM:  2500  Mode: TETAP
Kompresi: MATI
Status: AKTIF
```

#### Button Functions
- **PREV/NEXT**: Navigate wheel patterns, adjust RPM when held
- **HELP**: Cycle through control modes (TETAP/POT/SWEEP)
- **SAVE**: Store current settings to EEPROM
- **ABT**: Display system information and memory usage

#### Status Information
```
ArduStim v2.0
Flash: 29.6KB/30KB (96.5%)
RAM:   1.8KB/2KB (87.6%)
Uptime: 00:15:42
```

### Serial Commands
The original serial interface remains fully functional:
```
Available Commands:
R<rpm>     - Set RPM (e.g., R2500)
W<wheel>   - Select wheel pattern (e.g., W3)
M<mode>    - Set control mode (0=POT, 1=FIXED, 2=SWEEP)
S          - Save configuration
?          - Show help
```

### Supported Wheel Patterns
Over 60 patterns including:
- **60-2 Tooth Wheel** (Ford, VAG)
- **36-1 Tooth Wheel** (GM, BMW)
- **24-1 Tooth Wheel** (Nissan)
- **Dizzy 4/6/8 Cylinder** (Distributor simulation)
- **GM LS1 Crank+Cam** (58X + 4X)
- **Mitsubishi 4G63** (4+2 pattern)
- **Honda D17** (No cam)
- **Subaru 6/7** (Boxer engines)
- And many more...

## Development

### Project Structure
```
ardustim/                   # Main source directory
├── ardustim.ino           # Application entry point
├── globals.h              # System constants and structures
├── wheel_defs.h           # Wheel pattern definitions
├── pin_config.h           # Hardware pin assignments
├── comms.cpp/h            # Serial communication
├── storage.ino/h          # EEPROM management
└── LCD Interface Module:
    ├── display_interface.h    # Hardware abstraction
    ├── lcd_display.cpp/h      # LCD implementation
    ├── lcd_manager.cpp/h      # Display coordination
    ├── button_manager.cpp/h   # Button handling
    └── ui_controller.cpp/h    # UI state machine
```

### Memory Optimization
- **PROGMEM**: Wheel patterns stored in flash memory
- **Packed Structures**: Minimize RAM usage
- **Conditional Compilation**: LCD interface can be disabled
- **Link-time Optimization**: Dead code elimination

### Adding New Wheel Patterns
1. Define pattern array in `wheel_defs.h`
2. Add friendly name string
3. Update `Wheels[]` array with pattern parameters
4. Increment `MAX_WHEELS` constant

## Technical Specifications

### Timing Accuracy
- **Timer Resolution**: 16-bit hardware timer (Timer1)
- **Frequency Range**: 0.1 Hz to 10 kHz output frequency
- **RPM Range**: 0-9000 RPM (pattern dependent)
- **Jitter**: <1μs typical

### Memory Usage
```
Flash Memory (30KB usable):
├── Total Used: 29,646 bytes (96.5%)
├── Available:  1,074 bytes (3.5%)
└── Optimized for Nano's 30KB constraint

RAM (2KB total):
├── Used: 1,795 bytes (87.6%)
├── Available: 253 bytes (12.4%)
└── Efficient memory management
```

### Performance Characteristics
- **LCD Update Rate**: <100ms for critical changes
- **Button Response**: <50ms after debounce
- **Pattern Generation**: No timing degradation
- **Serial Baud Rate**: 115200 bps

## Troubleshooting

### Common Issues

#### LCD Not Working
```bash
# Check I2C connection
pio run -t upload -e nano
# Upload i2c_scanner.ino to verify LCD address
```

#### Memory Issues
```bash
# Check flash usage
pio run -e nano -v | grep "Memory Usage"

# Disable LCD interface if needed
# Comment out: -DENABLE_LCD_INTERFACE=1
```

#### Pattern Generation Problems
- Verify output pins (D8, D9, D10)
- Check serial monitor for error messages
- Ensure stable 5V power supply

### Debug Tools
- **Serial Monitor**: Real-time status and error messages
- **I2C Scanner**: Verify LCD hardware connection
- **Memory Monitor**: Track RAM usage in status display

## Contributing

### Development Guidelines
- Follow existing code style (2-space indentation)
- Add Doxygen comments for public APIs
- Test on Arduino Nano hardware
- Verify memory constraints
- Maintain serial protocol compatibility


## License

This project is licensed under the GNU General Public License v3.0 - see the original ArduStim license for details.

## Acknowledgments

- **Original ArduStim**: David J. Andruczyk and contributors
- **LCD Enhancement**: Custom implementation for Arduino Nano
- **Community**: Arduino and PlatformIO ecosystems

## Version History

- **v2.0**: Enhanced LCD interface with 20x4 display support
- **v1.x**: Original ArduStim with serial interface

---

**Note**: This is a personal fork optimized for Arduino Nano with LCD interface. For the original ArduStim project, visit the upstream repository.