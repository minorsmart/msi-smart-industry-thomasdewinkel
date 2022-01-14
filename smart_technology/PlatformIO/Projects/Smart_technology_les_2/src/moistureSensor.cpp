#include "moistureSensor.hpp"

MoistureSensor::MoistureSensor(int moistureSensorPin) : moistureSensorPin(moistureSensorPin)
{
}

MoistureSensor::~MoistureSensor()
{
}

int MoistureSensor::getMoistSensorValue()
{
    return analogRead(this->moistureSensorPin);
}
