#include "master.h"


master::master()
{ 
  curAddr = 0;
  busSetup();
}

void master::busSetup(void)
{
  pinMode(PinLED, OUTPUT);   
  pinMode(RS485Control, OUTPUT);    
  
  digitalWrite(RS485Control, RS485Receive);  // Init Transceiver   
  
  Serial1.begin(baud);   // set the data rate 
  
  for (byte i = 0 ; i < numSlaves; i++){
    state[i] = 0;
    slavedata[i][0] = 48;
    slavedata[i][1] = 48;
    slavedata[i][2] = 48;
    slavedata[i][3] = 48;
  }
}

void master::stateMachine(void)
{
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
  Serial1.flush();
  //Send ENQ to slaves
  if ( curAddr < 10 ) {
    sendMSG(48,i+48,68,48,48,32,slavedata[curAddr][0],slavedata[curAddr][1],slavedata[curAddr][2],slavedata[curAddr][3]);
  } else {
    sendMSG(49,i+48,68,48,48,32,slavedata[curAddr][0],slavedata[curAddr][1],slavedata[curAddr][2],slavedata[curAddr][3]);
  }
  //Wait for reply
  wait_time = 1000000 / baud * packetSize * 8 * 2;
  wait_time = wait_time/1000;
  previous_time=millis();
  while (((millis()-previous_time)<wait_time) && (Serial1.available()<15)){
  }
  //delay(2000);
  //Did we receive a packet?
  if (Serial1.available()>=15){
    if (receiveMSG()==1){
      //Did we receive a ACK and from this slave?
      address=(hex2num(rxdata[0])<<4)+(hex2num(rxdata[1]));
      if ((rxdata[2]==tACK) && (address == curAddr )){
        //Read data back
        if (rxdata[10] == 49){
          state[curAddr]=!state[curAddr];
        }  
      }
    }
  }

  curAddr++;
  if (curAddr >= numSlaves)
    curAddr=0;
}


byte master::receiveMSG(){

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



void master::sendMSG(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tENQ,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4); 
}

void master::sendACK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tACK,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4);
}

void master::sendNAK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){
  sendData(tNAK,address1,address2,data_type,code1,code2,Sign,data1,data2,data3,data4);
}

void master::sendData(byte type, byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4){

  unsigned int checksum_TX;
  checksum_TX=address1+address2+type+data_type+code1+code2+Sign+data1+data2+data3+data4+3;

  //UCSR0A=UCSR0A |(1 << TXC0);
  

  digitalWrite(RS485Control, RS485Transmit);  // Enable RS485 Transmit
  //digitalWrite(PinLED,HIGH);
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
  //while (!(UCSR0A & (1 << TXC0)));
  Serial1.flush();
  digitalWrite(RS485Control, RS485Receive);
  //digitalWrite(PinLED,LOW);  // Disable RS485 Transmit   
}



byte master::hex2num(byte x){

  byte result;

  if (x>=48 && x<=57){
    result=x-48;  
  }
  else if (x>=65 && x<=70){
    result=x-55;    
  }
  return result;  
}

