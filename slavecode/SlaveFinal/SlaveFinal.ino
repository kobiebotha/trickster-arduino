#include <CapacitiveSensor.h>

#define RS485Control     17   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define PinLED           13

#define PinADDR0         9
#define PinADDR1         8
#define PinADDR2         7
#define PinADDR3         6

#define PinINPUT3        5
#define PinINPUT2        4
#define PinINPUT1        3
#define PinINPUT0        2

#define capSensePinSend  15
#define capSensePinRead  16
#define capSenseWait     20
#define capSenseSamples  10

#define tNAK             15
#define tENQ             5
#define tACK             6

#define ledB             10
#define ledG             11
#define ledR             12

#define numPixels        1
#define wait             80

int  myID;
byte  binput;

byte  data[12];
byte  address;
byte  function;
byte  function_code;
unsigned int data_received;
byte  byte_receive;
byte  state=0;
byte  cont1=1;
byte  trace_OK=0;
unsigned int checksum;
unsigned int checksum_trace;
boolean ledState;
int   j;
unsigned long waitTimeLED;
unsigned long waitTimeCapSense;
long  capReading=0;
CapacitiveSensor   capSense = CapacitiveSensor(capSensePinSend,capSensePinRead); 

void setup() {
  
  capSense.set_CS_AutocaL_Millis(0xFFFFFFFF);
  
  pinMode(PinLED,OUTPUT);
  pinMode(RS485Control,OUTPUT);
  digitalWrite(RS485Control,RS485Receive);
  
  Serial.begin(4800);
  
  pinMode(ledB,OUTPUT);
  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  
  pinMode(PinADDR0,INPUT_PULLUP);
  pinMode(PinADDR1,INPUT_PULLUP);
  pinMode(PinADDR2,INPUT_PULLUP);
  pinMode(PinADDR3,INPUT_PULLUP);
  
  pinMode(PinINPUT0,INPUT_PULLUP);
  pinMode(PinINPUT1,INPUT_PULLUP);
  pinMode(PinINPUT2,INPUT_PULLUP);
  pinMode(PinINPUT3,INPUT_PULLUP);

  myID = 0;
  myID = myID + !digitalRead(PinADDR0);
  myID = myID + 2 * !digitalRead(PinADDR1);
  myID = myID + 4 * !digitalRead(PinADDR2); 
  myID = myID + 8 * !digitalRead(PinADDR3);   
  
  j = 0;
  ledState = true; 
  waitTimeLED = millis();
  waitTimeCapSense = millis();
}

void loop()
{
  
  if ((digitalRead(PinINPUT0)==LOW) |(capReading>1000)){
     binput = 49; 
  }
  else {                                                                                                                                                                                               
     binput = 48;
  };
  
  while (Serial.available() > 0){
    
     byte_receive=Serial.read();
     if (byte_receive==00){
       state=1;
       checksum_trace=0;
       checksum=0;
       trace_OK=0;
       address=0;
       data_received=0;
       cont1=1;
     }else if (state==1 && cont1<=12){
       data[cont1-1]=byte_receive;
       checksum=checksum+byte_receive;
       cont1=cont1+1;
     }else if (state==1 && cont1==13){
       checksum_trace=byte_receive<<8;
       cont1=cont1+1;
     }else if (state==1 && cont1==14){
       checksum_trace=checksum_trace+byte_receive;
       cont1=cont1+1;
       state=0;
       if (checksum_trace==checksum){
         trace_OK=1;
     
         address=(hex2num(data[0])<<4)+(hex2num(data[1]));
         function=data[3];
         function_code=(hex2num(data[4])<<4)+(hex2num(data[5]));
         data_received=(hex2num(data[7])<<12)+(hex2num(data[8])<<8)+(hex2num(data[9])<<4)+(hex2num(data[10]));
         if (address==myID){
           if ((function=='D') && (function_code==0) && data[2]==tENQ){
             if (data_received==1){
               digitalWrite(PinLED,HIGH);
               ledState = true;
               sendACK(data[0],data[1],data[3],data[4],data[5],data[6],48,48,48,binput);
             }else if (data_received==0){
               digitalWrite(PinLED,LOW);
               ledState = false;
               j=0;
               sendACK(data[0],data[1],data[3],data[4],data[5],data[6],48,48,48,binput);
             }
           }
         }
       }else{
         sendNAK(data[0],data[1],data[3],data[4],data[5],data[6],48,48,48,binput);
       }
     }

  }
  if ((millis()-waitTimeLED)>=wait) {
    if (ledState){
      setPixelColor(1, Wheel( (j+1) % 255));
      j++;
    }
    else {
      setPixelColor(1,0,0,0);
    }
    waitTimeLED = millis();
  }
  if ((millis()-waitTimeCapSense)>=capSenseWait) {
    capReading =  capSense.capacitiveSensor(capSenseSamples);
    waitTimeCapSense = millis();
  }
}

void sendMSG(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tENQ,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4); 
}

void sendACK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tACK,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4);
}

void sendNAK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tNAK,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4);
}

void sendData(byte type, byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){

  unsigned int checksum_ACK;
  checksum_ACK=address1+address2+type+data_type+code1+code2+Sign+data1+data2+data3+data4+3;

  Serial.flush();
  //UCSR0A=UCSR0A |(1 << TXC0);

  digitalWrite(RS485Control, RS485Transmit);  // Enable RS485 Transmit 
  delay(1);

  Serial.write(0);
  Serial.write(address1);
  Serial.write(address2);
  Serial.write(type);
  Serial.write(data_type);
  Serial.write(code1);
  Serial.write(code2);
  Serial.write(Sign);
  Serial.write(data1);
  Serial.write(data2);
  Serial.write(data3);
  Serial.write(data4);  
  Serial.write(3);
  Serial.write(((checksum_ACK>>8)&255));
  Serial.write(((checksum_ACK)&255));
  //while (!(UCSR0A & (1 << TXC0)));
  Serial.flush();
  digitalWrite(RS485Control, RS485Receive);  // Disable RS485 Transmit   
}



byte hex2num(byte x){

  byte result;

  if (x>=48 && x<=57){
    result=x-48;  
  }
  else if (x>=65 && x<=70){
    result=x-55;    
  }
  return result;  
}


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setPixelColor(int i,uint32_t c)
{
  byte r,g,b;
   
  b=(byte)c;
  c>>=8;
  g=(byte)c;
  c>>=8;
  r=(byte)c;
  
  analogWrite(ledR,r);
  analogWrite(ledG,g);
  analogWrite(ledB,b);
}

void setPixelColor(int i,byte r, byte g, byte b)
{
  analogWrite(ledR,r);
  analogWrite(ledG,g);
  analogWrite(ledB,b);
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
