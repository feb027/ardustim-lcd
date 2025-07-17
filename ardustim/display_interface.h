/* vim: set syntax=c expandtab sw=2 softtabstop=2 autoindent smartindent smarttab : */
/*
 * Display Interface Abstraction Layer
 * 
 * Provides hardware-independent display interface for LCD functionality
 * Supports future upgrade to TFT displays through abstraction
 *
 * Part of Ardu-Stim LCD Interface Enhancement
 */
#ifndef __DISPLAY_INTERFACE_H__
#define __DISPLAY_INTERFACE_H__

#include <stdint.h>

/**
 * Abstract base class for display hardware abstraction
 * Allows easy switching between LCD and future TFT displays
 */
class DisplayInterface {
public:
    virtual ~DisplayInterface() {}
    
    /**
     * Initialize the display hardware
     * @return true if initialization successful, false otherwise
     */
    virtual bool init() = 0;
    
    /**
     * Clear the entire display
     */
    virtual void clear() = 0;
    
    /**
     * Set cursor position for next print operation
     * @param col Column position (0-based)
     * @param row Row position (0-based)
     */
    virtual void setCursor(uint8_t col, uint8_t row) = 0;
    
    /**
     * Print text string at current cursor position
     * @param text Null-terminated string to display
     */
    virtual void print(const char* text) = 0;
    
    /**
     * Print integer value at current cursor position
     * @param value Integer value to display
     */
    virtual void print(int value) = 0;
    
    /**
     * Check if display hardware is available and functional
     * @return true if display is available, false otherwise
     */
    virtual bool isAvailable() = 0;
};

#endif