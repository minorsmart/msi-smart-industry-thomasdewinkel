#include "Arduino.h"
#include <WiFi.h>
#include "rfTransmitter.hpp"
#include "dhtSensor.hpp"
#include "waterpump.hpp"
#include "moistureSensor.hpp"
#include "ldrSensor.hpp"

class Webserver
{

private:
    char *ssid; 
    char *password; 

public:
    Webserver(char *ssid, char *password);
    void setupWebserver();
    void startWebserver();
    void displayWebpage();
    ~Webserver();
};
