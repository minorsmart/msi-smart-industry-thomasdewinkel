#include "Arduino.h"

class MoistureSensor
{
private:
    int moistureSensorPin = 0;
    
public:
    MoistureSensor(int moistureSensorPin);
    ~MoistureSensor();

    int getMoistSensorValue();

};

