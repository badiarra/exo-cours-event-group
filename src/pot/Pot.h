#ifndef POT_H
#define POT_H
#include <Arduino.h>

class Pot 
{
  public:
    Pot(int pin, int thresh);
    bool isActive();
    void init();
  private:
    int _pin;
    int _thresh;
};

#endif