#include "dhtSensor.hpp"

DhtSensor::DhtSensor(int dhtPin, int dhtType, DHT dhtSensor) : dhtPin(dhtPin), dhtType(dhtType), dhtSensor(DHT(dhtPin,dhtType))
{
}

DhtSensor::~DhtSensor()
{
}

int DhtSensor::getDhtPin()
{
    return this->dhtPin;
}

int DhtSensor::getDhtType()
{
    return this->dhtType;
}


void DhtSensor::setupDht()
{
    this->dhtSensor.begin();
}

float DhtSensor::getTemperature()
{
    float dhtTemp = 0;
    if (!isnan(this->dhtSensor.readTemperature()))
    {
        dhtTemp = this->dhtSensor.readTemperature();
    }
    return dhtTemp;
}
float DhtSensor::getHumidity()
{
        float dhtHumidity = 0;
    if (!isnan(this->dhtSensor.readHumidity()))
    {
        dhtHumidity = this->dhtSensor.readHumidity();
    }
    return dhtHumidity;
}