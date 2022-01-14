#include <Arduino.h>

const int ldrPin = 39;

//setup code only runs once at startup
void setup() {
  Serial.begin(9600); //start a serial connection with a baud rate of 9600
  pinMode(ldrPin, INPUT); //define ldrPin as INPUT since we want to read it
}

    //loop runs all the time over and over again
void loop() {

  Serial.println(analogRead(ldrPin)); //print the read value
  delay(200); //wait 200ms
}