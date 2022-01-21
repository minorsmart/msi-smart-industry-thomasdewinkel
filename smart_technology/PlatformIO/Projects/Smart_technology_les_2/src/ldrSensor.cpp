#include "ldrSensor.hpp"

LdrSensor::LdrSensor(int ldrSensorPin) : ldrSensorPin(ldrSensorPin)
{
}

LdrSensor::~LdrSensor()
{
}

int LdrSensor::getLdrSensorValue()
{
    return analogRead(this->ldrSensorPin);
}

void LdrSensor::setupLdrSensor()
{
    pinMode(this->ldrSensorPin, INPUT);
}