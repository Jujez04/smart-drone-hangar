#ifndef __LCD_I2C__
#define __LCD_I2C__

#include "Display.h"
#include <LiquidCrystal_I2C.h>

/**
 * I2C LCD implementation using LiquidCrystal_I2C library
 */
class LcdI2C : public Display {
public:

    /**
     * Constructor
     * @param address I2C address (typically 0x27 or 0x3F)
     * @param cols Number of columns (typically 16 or 20)
     * @param rows Number of rows (typically 2 or 4)
     */
    LcdI2C(uint8_t address, int cols, int rows);

    void init() override;
    void clear() override;
    void setCursor(int col, int row) override;
    void print(const String& text) override;
    void printAt(const String& text, int col, int row) override;
    void turnOn() override;
    void turnOff() override;

    int getCols() const override { return cols; }
    int getRows() const override { return rows; }

private:
    LiquidCrystal_I2C* lcd;
    uint8_t address;
    int cols;
    int rows;
};

#endif