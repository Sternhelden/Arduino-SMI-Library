#include <smi.h>

int i;
int j;
byte phyaddrHB;
byte phyaddrLB;
byte rData[2] = {0, 0};
byte wData[2] = {0, 0};
//unsigned long time;
//unsigned long timeb;
//unsigned long timee;

SMI smi(5,4);

void setup(){
  Serial.begin(57600);
}

void loop() {
  int funcSelect;
  int temp[4] = {0, 0, 0, 0};
  int address[4] = {0, 0, 0, 0};
  int regValue[4] = {0, 0, 0, 0};
  Serial.print("Select function [1]read register [2]write register:");
  while(Serial.available() == 0){} // wait input
  if(Serial.available() >0) {
    funcSelect = Serial.read();
    funcSelect = asciiToHex(funcSelect);
    Serial.println(funcSelect, DEC);
    switch(funcSelect) {
      case 1:
           Serial.print("Please input the register address:");
           while(Serial.available() == 0){} // wait input
//           timeb = millis();
           readSerialData(temp, 4);
           resort(temp, 4);
           phyaddrHB=(temp[0]<<4)+temp[1];
           phyaddrLB=(temp[2]<<4)+temp[3];
           print0(phyaddrHB);
           print0(phyaddrLB);
           Serial.println();           
           Serial.println("reading...");
           smi.read(phyaddrHB, phyaddrLB, rData); //this function reads the number from the external eeprom
           Serial.print("from Register ");
           print0(phyaddrHB);
           print0(phyaddrLB);
           Serial.println();           
           Serial.print("value is ");
           for (i=0; i<2; i++) {
             print0(rData[i]);
           }
           Serial.println();
           Serial.println();
//           timee = millis();
//           time = timee-timeb;
//           Serial.println(time);
           break;
       case 2:
           Serial.print("Please input the register address:");
           while(Serial.available() == 0){} // wait input
           readSerialData(temp, 4);
           resort(temp, 4);
           phyaddrHB=(temp[0]<<4)+temp[1];
           phyaddrLB=(temp[2]<<4)+temp[3];
           print0(phyaddrHB);
           print0(phyaddrLB);
           Serial.println();  
           Serial.print("Please input the register value:");
           while(Serial.available() == 0){} // wait input   
//           timeb = millis();        
           readSerialData(temp, 4);
           resort(temp, 4);
           wData[0]=(temp[0]<<4)+temp[1];
           wData[1]=(temp[2]<<4)+temp[3];
           print0(wData[0]);
           print0(wData[1]);
           Serial.println();       
           Serial.println("writing...");
           smi.write(phyaddrHB, phyaddrLB, wData); //this function reads the number from the external eeprom
           Serial.print("to Register ");
           print0(phyaddrHB);
           print0(phyaddrLB);
           Serial.println();  
           Serial.print("with value, ");
           print0(wData[0]);
           print0(wData[1]);
           Serial.println();  
           smi.read(phyaddrHB, phyaddrLB, rData); //this function reads the number from the external eeprom
           Serial.print("after writing, the register value is ");
           for (i=0; i<2; i++) {
             print0(rData[i]);
           }
           Serial.println();
           Serial.println();
//           timee = millis();
//           time = timee-timeb;
//           Serial.println(time,DEC);           
           break;
    }
  }
  //clear serial RX buffer
  while(Serial.available()) {
    Serial.read();
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
