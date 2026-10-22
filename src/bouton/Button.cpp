#include "Button.h"

Button::Button(int pin) : _pin(pin) {
}

void Button::init() {
    pinMode(_pin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return digitalRead(_pin) == LOW;
}