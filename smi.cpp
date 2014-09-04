#include "Arduino.h"
#include "Smi.h"

Smi::Smi(int clockPin, int dataPin) {
	pinMode(clockPin, OUTPUT);
	_clockPin = clockPin;
	pinMode(dataPin, OUTPUT);
	_dataPin = dataPin;
}

void Smi::write(byte HB, byte LB, byte target[]) {
	//common_routine(); 
	start();
	//OP code
	opWrite();
	putAddress(HB, LB);   //send byte
	writeTurnA();
	sendData(target, 2);
}

void Smi::read(byte HB, byte LB, byte target[]) {
	start();
	//OP code
	opRead();
	putAddress(HB, LB);   //send byte
	readTurnA();
	getData(target, 2);
}

void Smi::clockPulse() {
   digitalWrite (_clockPin, HIGH);
   digitalWrite (_clockPin, LOW);  
}

void Smi::start() {
  //Preamble
   digitalWrite (_dataPin, HIGH);
   for (int smi_i=0; smi_i<32; smi_i++) {
    clockPulse();
   }
   //SFD
    digitalWrite (_dataPin, LOW);
    clockPulse();
    digitalWrite (_dataPin, HIGH);
    clockPulse();
 }
 
void Smi::opRead() {
  //data=10
   digitalWrite (_dataPin, HIGH);
    clockPulse();
   digitalWrite (_dataPin, LOW);
    clockPulse();
}

void Smi::opWrite() {
  //data=01
   digitalWrite (_dataPin, LOW);
    clockPulse();
   digitalWrite (_dataPin, HIGH);
    clockPulse();
}

void Smi::putAddress(byte HB, byte LB) {
	int smi_i;
  for (smi_i=1; smi_i>=0; smi_i--){
      if ((HB &(1<<smi_i))==0) {
        digitalWrite (_dataPin, LOW); 
      } else {
        digitalWrite (_dataPin, HIGH); 
      }
    clockPulse();
  }
  for (smi_i=7; smi_i>=0; smi_i--){   
    if ((LB &(1<<smi_i))==0) {
      digitalWrite (_dataPin, LOW); 
    } else {
      digitalWrite (_dataPin, HIGH); 
    }
    clockPulse();
  }
}

void Smi::readTurnA() {
  pinMode(_dataPin, INPUT);
  clockPulse();
}

void Smi::writeTurnA() {
  digitalWrite (_dataPin, HIGH);
  clockPulse();
  digitalWrite (_dataPin, LOW);
  clockPulse();
}

void Smi::getData(byte target[], int arraySize) {
	int smi_i, smi_j;
  for (smi_j=0; smi_j<2; smi_j++) {
    target[smi_j]=0; // reset data array    
    for (smi_i=7; smi_i>=0; smi_i--) {
      digitalWrite (_clockPin, HIGH);
      if (digitalRead(_dataPin)==HIGH) {
        target[smi_j]=(target[smi_j] + (1<<smi_i)); 
      }
      digitalWrite (_clockPin, LOW); 
    }
  }
  pinMode(_dataPin,OUTPUT);
  digitalWrite(_dataPin,HIGH);
}

void Smi::sendData(byte target[], int arraySize) {
	int smi_i, smi_j;
  for (smi_j=0; smi_j<arraySize; smi_j++) {
    for (smi_i=7; smi_i>=0; smi_i--) {
      if ((target[smi_j] & (1<<smi_i))==0) {
        digitalWrite (_dataPin, LOW);
      } else {
        digitalWrite (_dataPin, HIGH);
      }
       clockPulse();
    }
  }
}