#include "Arduino.h"

class Waterpump
{
private:
    int pumpPin = 0;
public:
    Waterpump(int pumpPin);
    ~Waterpump();

    int getPumpPin();

    void setupWaterPump();
    void on();
    void off();
};


