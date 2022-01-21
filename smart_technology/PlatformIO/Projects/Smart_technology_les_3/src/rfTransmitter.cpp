#include "rfTransmitter.hpp"

RfTransmitter::RfTransmitter(int rfPin, unsigned long transmitterId) : rfPin(rfPin), transmitterId(transmitterId)
{
}

RfTransmitter::~RfTransmitter()
{
}

int RfTransmitter::getRfPin()
{
    return this->rfPin;
}

unsigned long RfTransmitter::getTransmitterId()
{
    return this->transmitterId;
}

void RfTransmitter::sendRfSignalHigh(int rfPin, unsigned long transmitterId, int group, int dev, int repeat, int dimlevel)
{
    switchKaku(rfPin, transmitterId, group, dev, true, repeat);
}

void RfTransmitter::sendRfSignalLow(int rfPin, unsigned long transmitterId, int group, int dev, int repeat, int dimlevel)
{
    switchKaku(rfPin, transmitterId, group, dev, false, repeat);
}