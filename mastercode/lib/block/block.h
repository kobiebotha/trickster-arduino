#ifndef BLOCK_H
#define BLOCK_H
#include <Arduino.h>
//typedef unsigned char byte;

byte onewire_crc(byte oldcrc, byte newbyte);

class block
{
private:
  byte send_receive[32];
  byte position;
  byte length;
  byte crc;
  byte genuine_length;
  boolean processBlock(void);
  byte receive_byte;
  boolean start;
public:
  block();
  void addByte(byte b);
  void sendBlock(Stream* R);
  boolean receiveBlock(Stream* R);
  byte nextByte(void);
  byte getLength(void);
  void clearBuffer(void);
};

#endif

