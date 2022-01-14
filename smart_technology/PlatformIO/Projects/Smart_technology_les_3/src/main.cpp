#include "webserver.hpp"

Webserver webserver = Webserver("Thomas", "12345678");

//setup code only runs once at startup
void setup()
{
  Serial.begin(115200); //start a serial connection with a baud rate of 9600
  webserver.setupWebserver();

}

//loop runs all the time over and over again
void loop()
{
    webserver.startWebserver();

  // if (millis() - prevMillisLow >= interval - 1000)
  // {
  //   prevMillisLow = millis();
  //   rfTransmitter.sendRfSignalLow(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
  //   waterpump.off();
  // }

  // if (millis() - prevMillisHigh >= interval)
  // {
  //   prevMillisHigh = millis();
  //   rfTransmitter.sendRfSignalHigh(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);

  //   // float h = dhtSensor.getHumidity();
  //   // float t = dhtSensor.getTemperature();
  //   // Serial.println (h);
  //   // Serial.println (t);
  //   waterpump.on();
  // }
}