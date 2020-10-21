#include <Wire.h>

//#include <Encoder.h>
//#define ENCODER_DO_NOT_USE_INTERRUPTS
//Encoder myEnc(5, 6); //2 and 3 are interrupt pins in UNO

#include <TM1637Display.h>
#define CLK 10
#define DIOY1 9
#define DIOG 11
TM1637Display displayYellow1(CLK, DIOY1);
TM1637Display displayGreen(CLK, DIOG);

const byte interruptPin = 3;
int AC_LOAD = 8;             // Output to Opto Triac pin
volatile int dimming = 128;  // Dimming level (0-128)  0 = ON, 128 = OFF

char t[5]={};               //empty array where for the data comming from slave

volatile int tempSet = 50; //initial threshold temperature

void setup()
{
  Serial.begin(9600);
  
  pinMode(AC_LOAD, OUTPUT); // Set AC Load pin as output
  attachInterrupt(digitalPinToInterrupt(interruptPin), cross, FALLING);   
   
  displayYellow1.setBrightness(2); //BRIGHTEST = 7;
  displayYellow1.clear();
  displayGreen.setBrightness(2); 
  displayGreen.clear();

  Wire.begin();
}

void loop()  {
  dimming = map(analogRead(A0), 0, 1024, 0, 10000); //map from pot
  
  Wire.requestFrom(8, 5);   //gathers data comming from slave
  int i = 0; 
    while (Wire.available()) { 
    t[i] = Wire.read();   // every character that arrives stored in "t"
    i = i + 1;
    }
  
  displayYellow1.showNumberDecEx(atof(t)*100, 0b01000000, false);
  //displayGreen.showNumberDecEx(atof(t)*100, 0b01000000, false);
      
//  int newPosition = myEnc.read();
//  if (newPosition != tempSet) {
//    tempSet = newPosition;
//    Serial.println(tempSet);
//  }
 
  //Serial.println(dimming);
  //displayGreen.showNumberDecEx(tempSet, 0b01000000, false);
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
