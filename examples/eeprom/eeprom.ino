#include <smi.h>

int i;
int j;
byte regEEPROMHB = 0x03;
byte regECARLB = 0x1A;
byte regEDRLB = 0x1B;
byte EEPROMaddress;
byte phyaddrHB;
byte phyaddrLB;
byte rData[2] = {0, 0};
byte wData[2] = {0, 0};
byte valueECAR[2] = {0, 0};
//unsigned long time;
//unsigned long timeb;
//unsigned long timee;

SMI smi(5,4);

void setup(){
  Serial.begin(57600);
}

void loop() {
  int funcSelect;
  int temp[4] = {0, 0};
  int address[4] = {0, 0, 0, 0};
  int regValue[4] = {0, 0, 0, 0};
  int valueECAR[2] = {0, 0};
  //Switch between different EEPROM type
  Serial.print("Select EEPROM type: [1]93C46 [2]93C56/93C66:");
  while(Serial.available() == 0){} // wait input
  if(Serial.available() >0) {
    funcSelect = Serial.read();
    funcSelect = asciiToHex(funcSelect);
    Serial.println(funcSelect, DEC);
    eepromType(funcSelect);
  } 
  //Operation
  Serial.print("Select operation: [1]read [2]write:");
  while(Serial.available() == 0){} // wait input
  if(Serial.available() >0) {
    funcSelect = Serial.read();
    funcSelect = asciiToHex(funcSelect);
    Serial.println(funcSelect, DEC);
    operation(funcSelect);
  } 

  //clear serial RX buffer
  while(Serial.available()) {
    Serial.read();
  }
}

void eepromType(int _temp) {
  if (_temp == 2) {
    //93C56_93C66
    valueECAR[1] = 1<6;
    Serial.println("93C56/93C66 is selected.");
  } else {
    Serial.println("93C46 is selected.");
  }
}

void operation(int _temp) {
  int _address;
  switch(_temp) {
    case 1:
      //read
      getAddress();
      break;
    case 2:
      //write
      getAddress();
      break;
  }
}

void getAddress(void) {
  int _address;
  Serial.print("Please input the EEPROM address:");
  while(Serial.available() == 0){} // wait input
  if(Serial.available() >0) {
    _address = Serial.read();
    _address = asciiToHex(_address);
    Serial.println(_address, DEC);
    valueECAR[0] = _address;
  }
}

int asciiToHex(int target) {
  if ((47 < target) && (target < 58)) {
    target = target-'0';
  } else if ((64 < target) && (target < 71)) {
    target = target-55;
  } else if ((96 < target) && (target < 103)) {
    target = target-87;
  }
  return (target);
}

int resort(int target[], int arraySize) {
  for (i=0;i<arraySize;i++) {
    if (target[i]==-1) {
      for (j=i;j>=1;j--) {
        target[j]=target[j-1];
      }
      target[0]=0;
    }
  }
}

int readSerialData(int target[], int arraySize) {
   for (i=0; i<4; i++) {
     target[i]=Serial.read();
     target[i]=asciiToHex(target[i]);
     delayMicroseconds(200);
   }
}

void print0(byte target) {
  if (target < 16) {
    Serial.print("0");
  }
    Serial.print(target, HEX);
}
