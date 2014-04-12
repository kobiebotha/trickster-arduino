#include "block.h"

byte onewire_crc(byte oldcrc, byte newbyte) { 
	byte shift_reg, data_bit, sr_lsb, fb_bit, j; 
	shift_reg=oldcrc; 
	for(j=0; j<8; j++) { // for each bit 
		data_bit = (newbyte >> j) & 0x01; 
		sr_lsb = shift_reg & 0x01; 
		fb_bit = (data_bit ^ sr_lsb) & 0x01; 
		shift_reg = shift_reg >> 1; 
		if (fb_bit) 
		shift_reg = shift_reg ^ 0x8c; 
	} 
return(shift_reg); 
}

block::block()
{
  position=0;
  crc=0;
  genuine_length=0;
}

void block::clearBuffer(void)
{
  position=0;
  crc=0;
  genuine_length=0;
}

void block::addByte(byte b)
{
  switch(b)
  {
  case 0xFF: 
    send_receive[position]=0x00;
    send_receive[position+1]=0xFE;
    position+=2;
    break;
  case 0xFD: 
    send_receive[position]=0x00;
    send_receive[position+1]=0xFC;
    position+=2;
    break;
  case 0x00: 
    send_receive[position]=0x00;
    send_receive[position+1]=0x00;
    position+=2;
    break;
  default:
    send_receive[position]=b;
    position+=1;
    crc = onewire_crc(crc,b);
    return;
  }
  crc = onewire_crc(crc,0x00);
  crc = onewire_crc(crc,send_receive[position-1]);
}

void block::sendBlock(Stream* R)
{
  //requires a serial port
  
  if(position>0)
  {
    if(crc==0xFF||crc==0xFD)
    {
      send_receive[position]=0x00;
      position++;
      crc=onewire_crc(crc,0x00);
    }
    
    length=position;
    position=0;
    R->write(0xFF);
    for(;position<length;position++)
    {
      R->write(send_receive[position]);
    }
    R->write(crc);
    R->write(0xFD);
  }
}

//
boolean block::receiveBlock(Stream* R)
{
  //requires a serial port
  //Serial.println(R->dataIn());
  if(R->available())
  {
    if(position==32)
      position=0;
    receive_byte=R->read();
    if(receive_byte==0xFF)
    {
      start=HIGH;
      position=0;
      return LOW;
    }
    else if(start)
    {
      if(receive_byte==0xFD)
      {
        length=position;
        return processBlock();
      }
      send_receive[position]=receive_byte;
      position++;
      
    }
    return LOW;
  }
  else
    return LOW;
}

byte block::nextByte(void)
{
  if(position==length)
  {
    position=0;
  }
  if(send_receive[position]==0)
  {
    position+=2;
    return send_receive[position-1];
  }
  position+=1;
  return send_receive[position-1];
}

byte block::getLength(void)
{
  return genuine_length;
}

boolean block::processBlock(void)
{
  position=0;
  genuine_length=0;
  crc=0;
  for(;position<(length-1);position++)
  {
    crc=onewire_crc(crc,send_receive[position]);
  }
  if(crc!=send_receive[position])
    return LOW;
  for(position=0;position<(length-1);position++)
  {
    if(send_receive[position]==0x00)
    {
      position+=1;
      if(send_receive[position]==0xFC||send_receive[position]==0xFE)
      {
        send_receive[position]+=0x01;
      }
    }
    genuine_length+=1;
  }
  position=0;
}
