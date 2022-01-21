#include "DHT.h"

class DhtSensor
{
private:
    int dhtPin = 0;
    int dhtType = 0;    
    DHT dhtSensor;

public:
    DhtSensor(int dhtPin, int dhtType, DHT dhtSensor);
    ~DhtSensor();

    int getDhtPin();
    int getDhtType();

    void setupDht();
    float getTemperature();
    float getHumidity();
};