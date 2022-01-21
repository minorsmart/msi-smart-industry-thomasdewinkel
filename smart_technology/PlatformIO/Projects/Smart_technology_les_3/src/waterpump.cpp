#include "waterpump.hpp"

Waterpump::Waterpump(int pumpPin) : pumpPin(pumpPin)
{
}

Waterpump::~Waterpump()
{
}

int Waterpump::getPumpPin()
{
    return this->pumpPin;
}

void Waterpump::setupWaterPump()
{
    pinMode(this->pumpPin, OUTPUT);
}

void Waterpump::on()
{
    digitalWrite(this->pumpPin, HIGH);
}

void Waterpump::off()
{
    digitalWrite(this->pumpPin, LOW);
}