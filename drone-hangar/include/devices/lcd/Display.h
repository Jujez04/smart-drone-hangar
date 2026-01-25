#ifndef __DISPLAY__
#define __DISPLAY__

#include <Arduino.h>

/**
 * Abstract interface for display devices
 */
class Display {
public:

    /**
     * Initialize the display
     */
    virtual void init() = 0;

    /**
     * Clear the entire display
     */
    virtual void clear() = 0;

    /**
     * Set cursor position
     * @param col Column (0-based)
     * @param row Row (0-based)
     */
    virtual void setCursor(int col, int row) = 0;

    /**
     * Print text at current cursor position
     * @param text Text to display
     */
    virtual void print(const String& text) = 0;

    /**
     * Print text at specific position
     * @param text Text to display
     * @param col Column (0-based)
     * @param row Row (0-based)
     */
    virtual void printAt(const String& text, int col, int row) = 0;

    /**
     * Turn display on/off
     */
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;

    /**
     * Get display dimensions
     */
    virtual int getCols() const = 0;
    virtual int getRows() const = 0;
};

#endif