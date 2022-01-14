#include "switchKaKu.h"


class RfTransmitter
{
private:
    int rfPin = 0;
    unsigned long transmitterId = 0;

public:
    RfTransmitter(int rfPin, unsigned long transmitterId);
    ~RfTransmitter();

    int getRfPin();
    unsigned long getTransmitterId();

    void sendRfSignalHigh(int rfPin, unsigned long transmitterId, int group, int dev, int repeat, int dimlevel = -1);
    void sendRfSignalLow(int rfPin, unsigned long transmitterId, int group, int dev, int repeat, int dimlevel = -1);

};

