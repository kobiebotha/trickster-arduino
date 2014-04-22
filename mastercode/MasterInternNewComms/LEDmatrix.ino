#include <tonematrix.h>

extern byte serialData[16];
extern int modeIndex;
extern byte start;
int dataIn = 33;
int load = 35;
int clock = 37;
int ledCounter=0;
 
int maxInUse = 1;    //change this variable to set how many MAX7219's you'll use
 
int e = 0;           // just a varialble
 
                     // define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

void matrixSetup(){
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);
 //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
 
}
void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a     //single max7219
 
  digitalWrite(load, LOW);       // begin    
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data  
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load,HIGH);
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin    
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}
 
void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for adressing different MAX7219's,
//whilele having a couple of them cascaded
 
  int c = 0;
  digitalWrite(load, LOW);  // begin    
 
  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }
 
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
 
  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }
 
  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load,HIGH);
}

void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void led_Loop(void)
{
  if (start==0)
  {
     maxSingle(1,0);
     maxSingle(2,0);
     maxSingle(3,0);
     maxSingle(4,0);                       //  - - - + - - - -
     maxSingle(5,0);
     maxSingle(6,0);
     maxSingle(7,0);
     maxSingle(8,0);   
  }
  
  if(modeIndex==0 && start<4 && start>0)
  {
    ledCounter++; 
   if(ledCounter<=25)
   {
     maxSingle(1,0);
     maxSingle(2,0);
     maxSingle(3,0);
     maxSingle(4,24);                       //  - - - + - - - -
     maxSingle(5,24);
     maxSingle(6,0);
     maxSingle(7,0);
     maxSingle(8,0); 
   }     
   if(ledCounter>25 && ledCounter<=50 )
   {
     maxSingle(1,0);
     maxSingle(2,0);
     maxSingle(3,60);                      
     maxSingle(4,36);
     maxSingle(5,36);                      
     maxSingle(6,60);
     maxSingle(7,0);
     maxSingle(8,0);
   }
   if(ledCounter>50 && ledCounter<=75 )
   {
     maxSingle(1,0);
     maxSingle(2,126);
     maxSingle(3,66);                      
     maxSingle(4,66);
     maxSingle(5,66);                      
     maxSingle(6,66);
     maxSingle(7,126); 
     maxSingle(8,0);     
   }
   if(ledCounter>75 && ledCounter<100 )
   {
     maxSingle(1,255);
     maxSingle(2,129);
     maxSingle(3,129);                      
     maxSingle(4,129);
     maxSingle(5,129);                      
     maxSingle(6,129);
     maxSingle(7,129);
     maxSingle(8,255);     
    
   }
   if(ledCounter>100)
    {
      ledCounter=0;
      start++;
    }
  
     
  }
  if(modeIndex==1 && start<4 && start>0)
  {
    ledCounter++; 
   if(ledCounter<=25)
   {
     maxSingle(1,15);
     maxSingle(2,14);
     maxSingle(3,12);
     maxSingle(4,8);                       //  - - - + - - - -
     maxSingle(5,16);
     maxSingle(6,48);
     maxSingle(7,112);
     maxSingle(8,240); 
   }     
   if(ledCounter>25 && ledCounter<=50 )
   {
     maxSingle(1,240);
     maxSingle(2,112);
     maxSingle(3,48);                      
     maxSingle(4,16);
     maxSingle(5,8);                      
     maxSingle(6,12);
     maxSingle(7,14);
     maxSingle(8,15);
   }
   if(ledCounter>50 && ledCounter<=75 )
   {
     maxSingle(1,128);
     maxSingle(2,192);
     maxSingle(3,224);                      
     maxSingle(4,240);
     maxSingle(5,15);                      
     maxSingle(6,7);
     maxSingle(7,3); 
     maxSingle(8,1);     
   }
   if(ledCounter>75 && ledCounter<100 )
   {
     maxSingle(1,1);
     maxSingle(2,3);
     maxSingle(3,7);                      
     maxSingle(4,15);
     maxSingle(5,240);                      
     maxSingle(6,224);
     maxSingle(7,192);
     maxSingle(8,128);     
    
   }
   if(ledCounter>100)
    {
      ledCounter=0;
      start++;
    }
  
     
  }
  if(start>=4)
  {
     maxSingle(1,serialData[1]);
     maxSingle(2,serialData[3]);
     maxSingle(3,serialData[5]);                      
     maxSingle(4,serialData[7]);
     maxSingle(5,serialData[9]);                      
     maxSingle(6,serialData[11]);
     maxSingle(7,serialData[13]);
     maxSingle(8,serialData[15]);   
  }
  
}
