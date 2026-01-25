#include "devices/lcd/LcdI2C.h"

LcdI2C::LcdI2C(uint8_t address, int cols, int rows)
    : address(address), cols(cols), rows(rows) {
    lcd = new LiquidCrystal_I2C(address, cols, rows);
}

void LcdI2C::init() {
    lcd->init();
    lcd->backlight();
    clear();
}

void LcdI2C::clear() {
    lcd->clear();
}

void LcdI2C::setCursor(int col, int row) {
    lcd->setCursor(col, row);
}

void LcdI2C::print(const String& text) {
    lcd->setCursor(0, 0);
    lcd->print(text);
}

void LcdI2C::printAt(const String& text, int col, int row) {
    lcd->setCursor(col, row);
    lcd->print(text);
}

void LcdI2C::turnOn() {
    lcd->display();
    lcd->backlight();
}

void LcdI2C::turnOff() {
    lcd->noDisplay();
    lcd->noBacklight();
}