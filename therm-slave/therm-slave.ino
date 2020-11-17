#include <Wire.h>

#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define THERMOMETERS_COUNT 3
#define THERMOMETER_ADDRESS_SIZE 8

int thermometerIndices[THERMOMETERS_COUNT];

const uint8_t THERMOMETERS_ADDRESSES[THERMOMETERS_COUNT][THERMOMETER_ADDRESS_SIZE] = { 
  {0x28, 0xAA, 0x39, 0x79, 0xA2, 0x00, 0x03, 0x9A},
  {0x28, 0x46, 0x74, 0x79, 0xA2, 0x05, 0x03, 0xCC},
  {0x28, 0x0F, 0x47, 0x79, 0xA2, 0x00, 0x03, 0xE2}
};

//{0x28, 0xAA, 0x39, 0x79, 0xA2, 0x00, 0x03, 0x9A} // sens. num 1
//{0x28, 0x46, 0x74, 0x79, 0xA2, 0x00, 0x03, 0xCB} // sens. num 2
//{0x28, 0x0F, 0x47, 0x79, 0xA2, 0x00, 0x03, 0xE2} // sens. num 3
//{0x28, 0xDE, 0x7A, 0x79, 0xA2, 0x00, 0x03, 0x79} // sens. num 4
//{0x28, 0x91, 0xA2, 0x79, 0xA2, 0x00, 0x03, 0x48} // sens. num 5 (wired and packaged)

char temp[5];

void setup() {
  delay(5000);
  
  Serial.begin(9600);
  
  sensors.begin(); 

  int count = sensors.getDeviceCount();
  if (count != THERMOMETERS_COUNT){
    Serial.print("Wrong thermometers count");
    exit(1);
  }
  
  for (int i=0; i<count; i++){
    DeviceAddress address;
    sensors.getAddress(address, i);
    for (int j=0; j<=count; j++){
      if (arraysEqual(address, THERMOMETERS_ADDRESSES[j], THERMOMETER_ADDRESS_SIZE)){
        thermometerIndices[j] = i;
        Serial.print("sensor ");
        Serial.print(i);
        Serial.print(" address ");
        printAddress(address);
        Serial.println(" check pass");
        break;
      }
      if (count == j) {  
      Serial.print("sensor ");
      Serial.print(i);
      Serial.print(" address ");
      printAddress(address);
      Serial.println(" check fail");
      thermometerIndices[i] = -1; 
      }
    }
  }

  Wire.begin(8);                          //Begins I2C communication with Slave Address as 8
  Wire.onRequest(requestEvent);           //Function call when Master request value from Slave
}

bool arraysEqual(const uint8_t arr1[], const uint8_t arr2[], const int size){
  for (int i=0; i<size; i++){
    if (arr1[i] != arr2[i]){
      return false;
    }
  }
  return true;
}

int getThermometerIndex(int thermometerId){
  return thermometerIndices[thermometerId];
}

void loop() {

  sensors.requestTemperatures(); 

  float temp0 = sensors.getTempCByIndex(getThermometerIndex(0));
  float temp1 = sensors.getTempCByIndex(getThermometerIndex(1));
  float temp2 = sensors.getTempCByIndex(getThermometerIndex(2));
  
  DeviceAddress address0, address1, address2;
  sensors.getAddress(address0, 0);
  sensors.getAddress(address1, 1);
  sensors.getAddress(address2, 2);
  
  Serial.print("thermometer 0: ");
  Serial.print(temp0);
  Serial.print("  address: ");
  printAddress(address0);
  Serial.println("");
  
  Serial.print("thermometer 1: ");
  Serial.print(temp1);
  Serial.print("  address: ");
  printAddress(address1);
  Serial.println("");
  
  Serial.print("thermometer 2: ");
  Serial.print(temp2);
  Serial.print("  address: ");
  printAddress(address2);
  Serial.println("");

  delay(100);
}

void requestEvent()      //This Function is called when Master wants value from slave
{
  Wire.write(temp);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
