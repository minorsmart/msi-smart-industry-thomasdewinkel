#include "rfTransmitter.hpp"
#include "dhtSensor.hpp"
#include "waterpump.hpp"
#include "moistureSensor.hpp"
#include "ldrSensor.hpp"

#define RFTRANSMITTERID 47115
#define DHTTYPE DHT11
#define DHTPIN 32

//Set pin number
int moistureSensorPin = 36;
int pumpPin = 33;
int rfPin = 25;
int ldrSensorPin = 39;

unsigned long prevMillisLow = 0;
unsigned long prevMillisHigh = 0;
unsigned long interval = 3000;

DHT dht(DHTPIN, DHTTYPE);

RfTransmitter rfTransmitter = RfTransmitter(rfPin, RFTRANSMITTERID);
DhtSensor dhtSensor = DhtSensor(DHTPIN, DHTTYPE, dht);
Waterpump waterpump = Waterpump(pumpPin);
MoistureSensor moistureSensor(moistureSensorPin);
LdrSensor ldrSensor = LdrSensor(ldrSensorPin);

//setup code only runs once at startup
void setup()
{
  Serial.begin(9600); //start a serial connection with a baud rate of 9600
  waterpump.setupWaterPump();
  dhtSensor.setupDht();
  ldrSensor.setupLdrSensor();
}

//loop runs all the time over and over again
void loop()
{

  if (millis() - prevMillisLow >= interval - 1000)
  {
    prevMillisLow = millis();
    rfTransmitter.sendRfSignalLow(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);
    waterpump.off();
  }

  if (millis() - prevMillisHigh >= interval)
  {
    prevMillisHigh = millis();
    rfTransmitter.sendRfSignalHigh(rfTransmitter.getRfPin(), rfTransmitter.getTransmitterId(), 1, 1, 3);

    // float h = dhtSensor.getHumidity();
    // float t = dhtSensor.getTemperature();
    // Serial.println (h);
    // Serial.println (t);
    waterpump.on();
  }
}