/*
  HC05 - Bluetooth AT-Command mode
  modified on 10 Feb 2019
  by Saeed Hosseini
  https://electropeak.com/learn/guides
*/
#define enablePin 9
#define powerPin 8

#include "SoftwareSerial.h"
//SoftwareSerial MyBlue(2, 3); // RX | TX
void setup()
{
  Serial.begin(9600);
  initHC05TransferMode();
  //initHC05ATMode();
}
void loop()
{
  //from bluetooth to Terminal.
  while (Serial1.available())
    Serial.write(Serial1.read());
  //from termial to bluetooth
  while (Serial.available())
    Serial1.write(Serial.read());
}

void initHC05ATMode() {
  String response = "";
  pinMode(enablePin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(enablePin, LOW);
  digitalWrite(powerPin, LOW);
  delay(1000);
  digitalWrite(enablePin, HIGH);
  delay(500);
  digitalWrite(powerPin, HIGH);
  delay(1000);
  Serial1.begin(38400);
  delay(1000);
  Serial1.write("AT\r\n");
  delay(1000);
  while (Serial1.available()) {
    response += String(char(Serial1.read()));
    Serial.println(response);
    delay(1000);
  }
  if (response == "OK\r\n") {
    Serial.println("AT Mode OK");
  } else {
    Serial.println("Response: '" + response + "' AT Mode Fail!");
  }
}

void initHC05TransferMode() {
  pinMode(enablePin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(enablePin, LOW);
  digitalWrite(powerPin, LOW);
  delay(500);
  digitalWrite(powerPin, HIGH);
  delay(1000);
  Serial1.begin(9600);
  Serial1.println("Welcome to eBraille :)...\nNow I am Ready to receive!!!");
}
