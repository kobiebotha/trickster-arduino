#include <LEDmatrix.h>

int latchPin = 30;
int latchPinSG = 31;
byte start=0;
//Pin connected to SH_CP of 74HC595
int clockPin = 27;
int clockPinSG = 29;

////Pin connected to DS of 74HC595
int dataPin = 23;
int dataPinSG = 25;
byte row1=0xF0;
byte row2=0xF0;
byte row3=0xF0;
byte row4=0xF0;
byte rowLED1=0x80;
byte rowLED2=0x40;
byte rowLED3=0x20;
byte rowLED4=0x10;
byte btnTEMP1=0;
byte btnTEMP2=0;
byte btnTEMP3=0;
byte btnTEMP4=0;
byte i=0;

void keypadSetup(){
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(latchPinSG, OUTPUT);
  pinMode(22, INPUT); 
  pinMode(24, INPUT); 
  pinMode(26, INPUT); 
  pinMode(28, INPUT); 
}

void LEDcontrol()
{
   byte tempRow1=0;
   byte tempRow2=0;
   byte tempRow3=0;
   byte tempRow4=0;
   byte lightOn=0;
   
 row1=changeState(0x07,row1);  
 row2=changeState(0x0B,row2);
 row3=changeState(0x0D,row3);
 row4=changeState(0x0E,row4);
 tempRow1=row1;
 tempRow2=row2;
 tempRow3=row3;
 tempRow4=row4;
 
 

  digitalWrite(latchPin, 0);
  rowLED1=0x10;
  tempRow1=~tempRow1; 
  rowLED1=rowLED1+tempRow1;
 shiftOut(dataPin, clockPin, rowLED1); 
 
 
 digitalWrite(latchPin,1);
delayMicroseconds(3000);

 

 digitalWrite(latchPin, 0);
  rowLED2=0x80;
  tempRow2=~tempRow2; 
  rowLED2=rowLED2+tempRow2;
 shiftOut(dataPin, clockPin, rowLED2); 
 digitalWrite(latchPin,1);
 delayMicroseconds(3000);

 digitalWrite(latchPin, 0);
   rowLED4=0x20;
   tempRow4=~tempRow4; 
   rowLED4=rowLED4+tempRow4;
    shiftOut(dataPin, clockPin, rowLED4);    
 digitalWrite(latchPin,1);
delayMicroseconds(3000);
 

 digitalWrite(latchPin, 0);
  rowLED3=0x40;
  tempRow3=~tempRow3; 
  rowLED3=rowLED3+tempRow3;
 shiftOut(dataPin, clockPin, rowLED3); 
 digitalWrite(latchPin,1);
 //delayMicroseconds(1000);

}


byte changeState(byte col,byte row)
{
  byte temp=0;
  byte tempRow=row;
  byte temp4=digitalRead(22);
  byte temp3=digitalRead(24);
  byte temp2=digitalRead(26);
  byte temp1=digitalRead(28); 
  
  
   digitalWrite(latchPinSG, 0);
    shiftOut(dataPinSG, clockPinSG, col); 
   
   if(!temp1)
   {
     if(temp1==btnTEMP1)
     {
      temp=0;
      temp=tempRow<<7;
      if(temp==0x80)
       tempRow=tempRow-0x01;
      else tempRow=tempRow+0x01;   
     }     
    btnTEMP1=temp1;
   }
   if(!temp2)
   {
     
     if(temp2==btnTEMP2)
     {
     temp=0;
     temp=tempRow<<6;
     if(temp>=0x80 && temp<=0xC0)
     tempRow=tempRow-0x02;
     else tempRow=tempRow+0x02;
     }    
     btnTEMP2=temp2;
   }
   if(!temp3)
  {    
     if(temp3==btnTEMP3)
     {
     temp=0;
     temp=tempRow<<5;
     if(temp>=0x80 && temp<=0xE0)
     tempRow=tempRow-0x04;
     else tempRow=tempRow+0x04;
     }    
     btnTEMP3=temp3;
  }
   if(!temp4)
   {     
     if(temp4==btnTEMP4)
     {
     temp=0;
     temp=tempRow<<4;
     if(temp>=0x80 && temp<=0xF0)
     tempRow=tempRow-0x08;
     else tempRow=tempRow+0x08;
     }     
     btnTEMP4=temp4;
  }
  digitalWrite(latchPinSG, 1);
  
  
  return tempRow; 
}
  

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}
