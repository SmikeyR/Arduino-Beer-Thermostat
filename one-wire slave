#include <Wire.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_A 5
#define ONE_WIRE_BUS_B 10
OneWire oneWireA(ONE_WIRE_BUS_A);
OneWire oneWireB(ONE_WIRE_BUS_B);
DallasTemperature sensorsA(&oneWireA);
DallasTemperature sensorsB(&oneWireB);

DeviceAddress insideThermometer = {0x28, 0xBE, 0x79, 0x79, 0xA2, 0x00, 0x03, 0xF4};
DeviceAddress tunThermometer1 = {0x28, 0xAA, 0x39, 0x79, 0xA2, 0x00, 0x03, 0x9A}; 
DeviceAddress tunThermometer2 = {0x28, 0x91, 0xA2, 0x79, 0xA2, 0x00, 0x03, 0x48};

char temp[5];

void setup() {
  Serial.begin(9600);
  sensorsA.begin();
  sensorsB.begin();

  Wire.begin(8);                          //Begins I2C communication with Slave Address as 8
  Wire.onRequest(requestEvent);           //Function call when Master request value from Slave
}

void loop() {
//  sensorsA.requestTemperaturesByAddress(insideThermometer);
//  float tempC = sensorsA.getTempC(insideThermometer);
//  Serial.print("temp in: ");
//  Serial.println(tempC);
//  sensorsB.requestTemperaturesByAddress(tunThermometer1);
//  Serial.print("temp tun 1: ");
//  Serial.println(sensorsB.getTempC(tunThermometer1));
//  sensorsB.requestTemperaturesByAddress(tunThermometer2);
//  Serial.print("temp tun 2: ");
//  Serial.println(sensorsB.getTempC(tunThermometer2));
//  delay(1000);
  sensorsA.requestTemperaturesByAddress(insideThermometer);
  float tempC = sensorsA.getTempC(insideThermometer);   
  dtostrf(tempC, 5, 2, temp);
  Serial.println(temp);
  delay(100);
}

void requestEvent()      //This Function is called when Master wants value from slave
{
  Wire.write(temp);
}
