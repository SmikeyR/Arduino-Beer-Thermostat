#include <Wire.h>

#include <Encoder.h>
//#define ENCODER_DO_NOT_USE_IdNTERRUPTS


/*
 * PINS SETUP:
 * 2, 5 - encoder. Why? Because pin 2 accepts interrupts, and encoder with first interrupt-accepting pin gives OK performance (https://www.pjrc.com/teensy/td_libs_Encoder.html)
 * 
 */




Encoder myEnc(2, 5); //2 and 3 are interrupt pins in UNO

#include <TM1637Display.h>
#define CLK 10
#define DIOY1 9
#define DIOG 11
TM1637Display displayYellow1(CLK, DIOY1);
TM1637Display displayGreen(CLK, DIOG);

const byte DIMMER_INTERRUPT_PIN = 3;
const int AC_LOAD = 8;             // Output to Opto Triac pin
volatile int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF


const byte SLAVE_BUFFER_SIZE = 5;
char slaveBuffer[SLAVE_BUFFER_SIZE]={};               //empty array where for the data comming from slave

const byte SLAVE_ADDRESS = 8;

volatile int tempSet = 50; //initial threshold temperature

void setup()
{
  Serial.begin(9600);
  
  setupPins();   
  setupDisplays();

  Wire.begin();
}

void setupPins(){
  pinMode(AC_LOAD, OUTPUT); // Set AC Load pin as output
  attachInterrupt(digitalPinToInterrupt(DIMMER_INTERRUPT_PIN), cross, FALLING);   
  
}

void setupDisplays(){
  displayYellow1.setBrightness(2); //BRIGHTEST = 7;
  displayYellow1.clear();
  displayGreen.setBrightness(2); 
  displayGreen.clear();
}



void loop()  {
  Serial.println("a");
  dimming = map(analogRead(A0), 0, 1024, 0, 10000); //map from pot
  Serial.println("aaaa");

  if (slaveConnected(SLAVE_ADDRESS)){
    Serial.println("slaveConnected returned true");
    Wire.requestFrom(SLAVE_ADDRESS, SLAVE_BUFFER_SIZE);   //gathers data comming from slave  
    int i = 0; 
    Serial.println("b");
    while (Wire.available()) { 
      slaveBuffer[i] = Wire.read();   // every character that arrives stored in "t"
      i = i + 1;
      if (i > SLAVE_BUFFER_SIZE){
        Serial.println("Trying to overflow the slave buffer, please see how the fuck this shit happened");
        break;
      }
    }
  } else {
    Serial.println("The fucking slave is sleeping, wake the motherfucker up!!!!!");
  }
  

  
  Serial.println("c");
  displayYellow1.showNumberDecEx(atof(slaveBuffer)*100, 0b01000000, false);
  //displayGreen.showNumberDecEx(atof(slaveBuffer)*100, 0b01000000, false);
      
//  int newPosition = myEnc.read();
  long encoderPosition = readEncoderValue();
  Serial.println("d");
  Serial.println(encoderPosition);

  delay(200);
//  if (newPosition != tempSet) {
//    tempSet = newPosition;
//    Serial.println(tempSet);
//  }
 
  //Serial.println(dimming);
  //displayGreen.showNumberDecEx(tempSet, 0b01000000, false);
}


bool slaveConnected(int address){
  return false;
  Serial.println("before beginTransmission");
  Wire.beginTransmission (address);
  Serial.println("after beginTransmission");
  return Wire.endTransmission() == 0;
}


long readEncoderValue(){
  return myEnc.read();
}


void cross()  //function to be fired at the zero crossing
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms 
  // Every zerocrossing thus: (50Hz)-> 10ms (1/2 Cycle) 
  // For 60Hz => 8.33ms (10.000/120)
  
  if (dimming < 9600 && dimming >= 850) {   
    delayMicroseconds(dimming);    // Wait till firing the TRIAC  // For 60Hz =>65
    digitalWrite(AC_LOAD, HIGH);   // Fire the TRIAC
    delayMicroseconds(10);         // triac On propogation delay. Some Triacs need a longer period
    digitalWrite(AC_LOAD, LOW);    // No longer trigger the TRIAC (the next zero crossing will switch it off) TRIAC
  }
  if (dimming < 850) {
    delayMicroseconds(850);
    digitalWrite(AC_LOAD, HIGH);
    delayMicroseconds(10);         
    digitalWrite(AC_LOAD, LOW);     
  }
}
