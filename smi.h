/*

*/
#ifndef Smi_h
#define Smi_h
#include "Arduino.h"

class Smi {
	public:

		Smi(int clockPin, int dataPin);
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