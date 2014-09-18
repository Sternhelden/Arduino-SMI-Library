/*
	Davicom Products SMI access library
	Author: Russell Liu
	Copyright: GPL V2
	
	This library provide a SMI access to read/write register 
	from/to the DMavicom products with SMI.
	
	2014-9-18 <russell_liu@davicom.com.tw>
*/
#define __PROG_TYPES_COMPAT__

#ifndef smi_h
#define smi_h

#if defined(ARDUINO) && ARDUINO >= 100 // Arduino 1.0
#include "Arduino.h"
#else
#include "WProgram.h" // Arduino 0022
#endif

class SMI {
	public:
		SMI(int clockPin, int dataPin);
		void read(byte HB, byte LB, byte target[]);
		void write(byte HB, byte LB, byte target[]);
	private:
		int _clockPin;
		int _dataPin;

		void clockPulse();
		void start();
		void opRead();
		void opWrite();
		void putAddress(byte HB, byte LB);
		void readTurnA();
		void writeTurnA();
		void getData(byte target[], int arraySize);
		void sendData(byte target[], int arraySize);
};

#endif