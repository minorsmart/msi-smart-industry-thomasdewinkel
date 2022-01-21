#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "rfTransmitter.hpp"
#include "dhtSensor.hpp"
#include <WebServer.h>

class Webserver
{

private:
    char *ssid; 
    char *password; 

public:
    Webserver(char *ssid, char *password);
    void setupWebserver();
    void startWebserver();

    static void gettemperature();
    ~Webserver();
};
