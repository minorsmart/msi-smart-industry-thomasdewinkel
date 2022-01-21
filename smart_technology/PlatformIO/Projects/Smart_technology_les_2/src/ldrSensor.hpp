#include "Arduino.h"

class LdrSensor
{
private:
    int ldrSensorPin = 0;
    
public:
    LdrSensor(int ldrSensorPin);
    ~LdrSensor();

    int getLdrSensorValue();
    
    void setupLdrSensor();

};

