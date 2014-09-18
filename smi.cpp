/*
	Davicom Products SMI access library
	Author: Russell Liu
	Copyright: GPL V2
	
	This library provide a SMI access to read/write register 
	from/to the DMavicom products with SMI.
	
	2014-9-18 <russell_liu@davicom.com.tw>
*/

#include "Arduino.h"
#include "smi.h"

SMI::SMI(int clockPin, int dataPin) {
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_clockPin = clockPin;
	_dataPin = dataPin;
}

void SMI::write(byte HB, byte LB, byte target[]) {
	start();
	//OP code
	opWrite();
	putAddress(HB, LB);     //send register address
	writeTurnA();			
	sendData(target, 2);
}

void SMI::read(byte HB, byte LB, byte target[]) {
	start();
	//OP code
	opRead();
	putAddress(HB, LB);  	//send register address
	readTurnA();			
	getData(target, 2);
}

// used to send a clock on clock pin
void SMI::clockPulse() {
   digitalWrite (_clockPin, HIGH);
   digitalWrite (_clockPin, LOW);  
}

// Preamble + SFD
void SMI::start() {
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
 
void SMI::opRead() {
  //data=10
   digitalWrite (_dataPin, HIGH);
    clockPulse();
   digitalWrite (_dataPin, LOW);
    clockPulse();
}

void SMI::opWrite() {
  //data=01
   digitalWrite (_dataPin, LOW);
    clockPulse();
   digitalWrite (_dataPin, HIGH);
    clockPulse();
}

// Send register address
void SMI::putAddress(byte HB, byte LB) {
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

void SMI::readTurnA() {
  pinMode(_dataPin, INPUT);
  clockPulse();
}

void SMI::writeTurnA() {
  digitalWrite (_dataPin, HIGH);
  clockPulse();
  digitalWrite (_dataPin, LOW);
  clockPulse();
}

void SMI::getData(byte target[], int arraySize) {
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

void SMI::sendData(byte target[], int arraySize) {
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