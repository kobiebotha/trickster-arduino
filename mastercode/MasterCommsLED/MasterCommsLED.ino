#define RS485Control     20   //RS485 Direction control

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

#define tNAK             15
#define tENQ             5
#define tACK             6

#define mybaud           14400
#define numSlaves        16
#define packetSize       15

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

byte            rxdata[packetSize-3];
byte            slavedata[numSlaves][4];
uint32_t        slaveState;
boolean         state[numSlaves];
byte            address;
unsigned long   previous_time;
unsigned long   wait_time;
char            prevBtn = 5;
char            incomingByte = 0;
byte            command = 0;        


void setup() {
  
  pinMode(PinLED, OUTPUT);   
  pinMode(RS485Control, OUTPUT);    
  
  digitalWrite(RS485Control, RS485Receive);  // Init Transceiver
  
  Serial.begin(9600);
  
  Serial1.begin(mybaud);   // set the data rate 
  
  pinMode(PinADDR0,INPUT_PULLUP);
  pinMode(PinADDR1,INPUT_PULLUP);
  pinMode(PinADDR2,INPUT_PULLUP);
  pinMode(PinADDR3,INPUT_PULLUP);
  
  pinMode(PinINPUT0,INPUT_PULLUP);
  pinMode(PinINPUT1,INPUT_PULLUP);
  pinMode(PinINPUT2,INPUT_PULLUP);
  pinMode(PinINPUT3,INPUT_PULLUP); 

  for (byte i = 0 ; i < numSlaves; i++){
    state[i] = 0;
    slavedata[i][0] = 48;
    slavedata[i][1] = 48;
    slavedata[i][2] = 48;
    slavedata[i][3] = 48;
  }
  slaveState = 0xF0F0F0F0;
  wait_time = 1000000 / mybaud * packetSize * 8 * 2;
  wait_time = wait_time/1000;
  wait_time = 40; // override to compensate for capsense
  
  LEDDisplaySetup();
}

void loop()
{
 
  //Sample inputs on main
  if (digitalRead(PinINPUT0)==LOW){
    state[0]=!state[0];
  }
  if (digitalRead(PinINPUT1)==LOW){
    state[1]=!state[1];
  }
  if (digitalRead(PinINPUT2)==LOW){
    state[2]=!state[2];
  }
  if (digitalRead(PinINPUT3)==LOW){
    state[3]=!state[3];
  }
  
  for (byte i = 0 ; i < numSlaves; i++){
    slavedata[i][0] = 48;
    slavedata[i][1] = 48;
    slavedata[i][2] = 48;
    if (state[i] == HIGH) {
      slavedata[i][3] = 49;
    } else {
      slavedata[i][3] = 48;
    }
  }

  //Transmit to Slaves
  //for thru slaves
  for (byte i = 0; i < numSlaves; i++) {
    Serial1.flush();
    //Send ENQ to slaves
    if ( i < 10 ) {
      sendMSG(48,i+48,68,48,48,32,(byte)slaveState,(byte)(slaveState>>8),(byte)(slaveState>>16),(byte)(slaveState>>24));      
    } else {
      sendMSG(49,i+48-10,68,48,48,32,(byte)slaveState,(byte)(slaveState>>8),(byte)(slaveState>>16),(byte)(slaveState>>24));
    }
    previous_time=millis();
    LEDDisplayLoop();
   // while (((millis()-previous_time)<wait_time) && (Serial1.available()<15)){
    while (((millis()-previous_time)<wait_time)){
    }
    //Did we receive a packet?
    if (Serial1.available()>=15){
      if (receiveMSG()==1){
        //Did we receive a ACK and from this slave?
         if (rxdata[0] == 49)
           address=10+hex2num(rxdata[1]);
         else
           address=hex2num(rxdata[1]);
        if ((rxdata[2]==tACK) && (address == i )){
          //Read data back
          if (rxdata[10] == 49)
            bitSet(slaveState,i+((i/4)*4));
          else
            bitClear(slaveState,i+((i/4)*4));
        }
      }
    }
  }
  keyLoop();
  sendPD();
}

byte receiveMSG(){

  byte  byte_receive;
  byte  state=0;
  byte  cont1=1;
  byte  trace_OK=0;

  unsigned int checksum;
  unsigned int checksum_trace;



  while (Serial1.available() > 0){

    byte_receive=Serial1.read();
    if (byte_receive==00){
      state=1;
      checksum_trace=0;
      checksum=0;
      trace_OK=0;
      cont1=1;
    }
    else if (state==1 && cont1<=12){
      rxdata[cont1-1]=byte_receive;
      checksum=checksum+byte_receive;
      cont1=cont1+1;
    }
    else if (state==1 && cont1==13){
      checksum_trace=byte_receive<<8;
      cont1=cont1+1;
    }
    else if (state==1 && cont1==14){
      checksum_trace=checksum_trace+byte_receive;
      cont1=cont1+1;
      state=0;
      if (checksum_trace==checksum){
        trace_OK=1;
      }
      else{
        trace_OK=0;
      }
      break;
    }
  }
  return trace_OK;

}

void sendPD(void){
  Serial.write(0x61);        
  Serial.write(byte(slaveState));
  Serial.write(byte(slaveState>>8));
  Serial.write(byte(slaveState>>16));
  Serial.write(byte(slaveState>>24));
  Serial.write(command|0xF0);
  Serial.write(0x62);
  
  if ((command == 2) | (command == 3))
    command = 0;
      
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

  unsigned int checksum_TX;
  checksum_TX=address1+address2+type+data_type+code1+code2+Sign+data1+data2+data3+data4+3;

  digitalWrite(RS485Control, RS485Transmit);  // Enable RS485 Transmit
  digitalWrite(PinLED,HIGH);
  delay(1);

  Serial1.write(0);
  Serial1.write(address1);
  Serial1.write(address2);
  Serial1.write(type);
  Serial1.write(data_type);
  Serial1.write(code1);
  Serial1.write(code2);
  Serial1.write(Sign);
  Serial1.write(data1);
  Serial1.write(data2);
  Serial1.write(data3);
  Serial1.write(data4);  
  Serial1.write(3);
  Serial1.write(((checksum_TX>>8)&255));
  Serial1.write(((checksum_TX)&255));
  Serial1.flush();
  digitalWrite(RS485Control, RS485Receive);
  digitalWrite(PinLED,LOW);  // Disable RS485 Transmit   
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


void keyLoop() 
{
  //The example shows using bytes on the serial port to move the menu. You can hook up your buttons or other controls.
  
  prevBtn = incomingByte;
  incomingByte = read_LCD_buttons();
  if ((incomingByte != btnNONE) & ( prevBtn != incomingByte))
  switch( incomingByte )
  {
    case btnUP:
      command = 2;
      break;
    case btnDOWN:
      command = 3;
      break;
    case btnRIGHT:
      command = 1;
      break;
    case btnLEFT:
      command = 0;    
      break;
    default:
      break;
  }
}

int read_LCD_buttons()
{
 int adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 return btnNONE;
}

