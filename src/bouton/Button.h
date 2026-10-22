#ifndef BUTYTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    int _pin;

public:
    Button(int pin);

    void init();
    bool isPressed();
};

#endif