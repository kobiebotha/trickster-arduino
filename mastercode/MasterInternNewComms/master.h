#ifndef MASTER_H
#define MASTER_H

#define RS485Control     20   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define PinLED           13

#define tNAK             15
#define tENQ             5
#define tACK             6

#define mybaud           4800
#define numSlaves        2
#define packetSize       15

byte            rxdata[packetSize-3];
byte            slavedata[numSlaves][4];
boolean         state[numSlaves];
byte            address;
unsigned long   previous_time;
unsigned long   wait_time;
byte            curAddr;


extern toneMatrix mine;
class master
{
  private:
    void busSetup(void);
    byte receiveMSG();
    void sendMSG(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4);
    void sendACK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4);
    void sendNAK(byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4);
    void sendData(byte type, byte address1,byte address2,byte data_type,byte code1,byte code2,byte Sign,byte data1,byte data2,byte data3,byte data4);
    byte hex2num(byte x);

  public:
    void stateMachine(void);
    master();
    
};

#endif
