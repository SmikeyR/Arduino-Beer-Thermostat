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


#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define THERMOMETERS_COUNT 3
#define THERMOMETER_ADDRESS_SIZE 8

int thermometerIndices[THERMOMETERS_COUNT];

const uint8_t THERMOMETERS_ADDRESSES[THERMOMETERS_COUNT][THERMOMETER_ADDRESS_SIZE] = { 
  {0x28, 0xAA, 0x39, 0x79, 0xA2, 0x00, 0x03, 0x9A  },
  {0x28, 0x46, 0x74, 0x79, 0xA2, 0x00, 0x03, 0xCB  },
  {0x28, 0x0F, 0x47, 0x79, 0xA2, 0x00, 0x03, 0xE2  }
};


char temp[5];

void setup() {

  sensors.begin(); 

  int count = sensors.getDeviceCount();
  if (count != THERMOMETERS_COUNT){
    Serial.print("Wrong thermometers count, you cunt");
    Serial.print("Fuck off!!!!!");
    exit(1);

  }
  for (int i=0; i<count; i++){
    uint8_t* address;
    sensors.getAddress(address, i);

    for (int j=0; j<count; j++){
      if (arraysEqual(address, THERMOMETERS_ADDRESSES[j], THERMOMETER_ADDRESS_SIZE)){
        thermometerIndices[j] = i;
        break;
      }
    }
  }


  Serial.begin(9600);
  sensorsA.begin();
  sensorsB.begin();

  Wire.begin(8);                          //Begins I2C communication with Slave Address as 8
  Wire.onRequest(requestEvent);           //Function call when Master request value from Slave
}

bool arraysEqual(const uint8_t arr1[], const uint8_t arr2[], const int size){
  for (int i=0; i<size; i++){
    if (arr1[i] != arr2[i]){
      return false;
    }
    return true;
  }
}

int getThermometerIndex(int thermometerId){
  return thermometerIndices[thermometerId];
}

void loop() {

  sensors.requestTemperatures(); 

  float temp0 = sensors.getTempCByIndex(getThermometerIndex(0));
  float temp1 = sensors.getTempCByIndex(getThermometerIndex(1));
  float temp2 = sensors.getTempCByIndex(getThermometerIndex(2));

  Serial.print("temperature of thermometer 0: ");
  Serial.println(temp0);
  Serial.print("temperature of thermometer 1: ");
  Serial.println(temp1);
  Serial.print("temperature of thermometer 2: ");
  Serial.println(temp2);

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
  // sensorsA.requestTemperaturesByAddress(insideThermometer);
  // float tempC = sensorsA.getTempC(insideThermometer);   
  // dtostrf(tempC, 5, 2, temp);
  // Serial.println(temp);
  delay(100);
}

void requestEvent()      //This Function is called when Master wants value from slave
{
  Wire.write(temp);
}
