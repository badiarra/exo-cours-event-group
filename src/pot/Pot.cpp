#include "Pot.h"

Pot::Pot(int pin, int thresh){
    _pin = pin;
    _thresh = thresh;
}
void Pot::init(){
    pinMode(_pin, INPUT);
}

bool Pot::isActive(){
    return analogRead(_pin) > _thresh;
}