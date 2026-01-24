#include "devices/servo/ServoMotorImpl.h"

ServoMotorImpl::ServoMotorImpl(int pin) {
    this->pin = pin;
    this->_on = false;
}

void ServoMotorImpl::on() {
    if (!_on) {
        motor.attach(pin);
        _on = true;
    }
}

bool ServoMotorImpl::isOn() {
    return _on;
}

void ServoMotorImpl::setPosition(int angle) {
    if (!_on) return;
    int targetAngle = constrain(angle, 0, 180);
    int pulseWidth = map(targetAngle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    motor.write(pulseWidth);
}

void ServoMotorImpl::off() {
    if (_on) {
        motor.detach();
        _on = false;
    }
}