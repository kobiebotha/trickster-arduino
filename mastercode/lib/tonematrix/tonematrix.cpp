#include "tonematrix.h"
#include <Arduino.h>
//#include <iostream>
//#include <bitset>
//address-------------------->
//0x00								0x01						0x02						0x03
const byte toneMatrix::tree[2][96]={
  {
    0,0x01,4,0x01,4,0x02,  0,0x02,0,0x04,4,0x04, 0,0x10,4,0x10,4,0x20, 0,0x20,0,0x40,4,0x40, //branch
    1,0x01,5,0x01,5,0x04,  1,0x04,1,0x08,5,0x08, 1,0x10,5,0x10,5,0x40, 1,0x40,1,0x80,5,0x80, //|
    2,0x01,6,0x01,6,0x02,  2,0x02,2,0x08,6,0x08, 2,0x10,6,0x10,6,0x20, 2,0x20,2,0x80,6,0x80, //|
    3,0x02,7,0x02,7,0x04,  3,0x04,3,0x08,7,0x08, 3,0x20,7,0x20,7,0x40, 3,0x40,3,0x80,7,0x80    }
  ,//V
  {
    0,0x01,4,0x01,4,0x02,	1,0x01,5,0x01,5,0x04,	2,0x01,6,0x01,6,0x02,	3,0x02,7,0x02,7,0x04,
    0,0x02,0,0x04,4,0x04,	1,0x04,1,0x08,5,0x08,	2,0x02,2,0x08,6,0x08, 3,0x04,3,0x08,7,0x08,
    0,0x10,4,0x10,4,0x20,	1,0x10,5,0x10,5,0x40,	2,0x10,6,0x10,6,0x20,	3,0x20,7,0x20,7,0x40,
    0,0x20,0,0x40,4,0x40,	1,0x40,1,0x80,5,0x80,	2,0x20,2,0x80,6,0x80,	3,0x40,3,0x80,7,0x80}
};

const byte toneMatrix::pad[2][32]={
  { 0,0x80,4,0x80,1,0x20,1,0x02,	0,0x08,4,0x08,5,0x20,5,0x02,
    7,0x10,7,0x01,6,0x40,2,0x40,	3,0x10,3,0x01,6,0x04,2,0x04}
  ,{1,0x20,2,0x40,3,0x10,5,0x20,  7,0x01,0,0x80,4,0x80,6,0x40,
    6,0x04,4,0x08,0,0x08,7,0x10,  5,0x02,3,0x01,2,0x04,1,0x02}
};

toneMatrix::toneMatrix()
{
  column[0]=0;
  column[1]=0;
  column[2]=0;
  column[3]=0;
  column[4]=0;
  column[5]=0;
  column[6]=0;
  column[7]=0;
  light_mode=SPIRAL;
}

void toneMatrix::setMode(mode m)
{
  if(m!=light_mode)
  {
    column[0]=0;
    column[1]=0;
    column[2]=0;
    column[3]=0;
    column[4]=0;
    column[5]=0;
    column[6]=0;
    column[7]=0;
    light_mode=m;
  }
}

void toneMatrix::fromTree(byte branch,byte address,byte state)
{
  byte state0,state1,state2;
  byte read_from=branch*24+(address*6);
  const byte *place =&tree[light_mode][read_from];
  state0=state&0x01;
  state1=state&0x02;
  state2=state&0x04;

  placement(place,state0);
  place=place+2;
  placement(place,state1);
  place=place+2;
  placement(place,state2);
}
void toneMatrix::fromPad(byte a,byte b, byte c, byte d)
{
  const byte* place=pad[light_mode];
  byte state;
  byte pos;
  for(pos=0x01;pos<0x09;pos=pos<<1)
  {
    state=a&pos;
    placement(place,state);
    place=place+2;
  }
  for(pos=0x01;pos<0x09;pos=pos<<1)
  {
    state=b&pos;
    placement(place,state);
    place=place+2;
  }
  for(pos=0x01;pos<0x09;pos=pos<<1)
  {
    state=c&pos;
    placement(place,state);
    place=place+2;
  }
  for(pos=0x01;pos<0x09;pos=pos<<1)
  {
    state=d&pos;
    placement(place,state);
    place=place+2;
  }
}

void toneMatrix::placement(const byte* place,byte isOn)
{
  if(isOn)
    column[*place]=column[*place]|*(place+1);
  else
    column[*place]=column[*place]&~(*(place+1));
}

void toneMatrix::print(Stream& s)
{
  s.println(column[0],BIN);
  s.println(column[1],BIN);
  s.println(column[2],BIN);
  s.println(column[3],BIN);
  s.println(column[4],BIN);
  s.println(column[5],BIN);
  s.println(column[6],BIN);
  s.println(column[7],BIN);
  s.println("");
}

const byte * toneMatrix::getBytes(void)
{
  return column;
}

/*void toneMatrix::print(void)
{
  std::cout<<std::bitset<8>(column[0])<<"\n";
  std::cout<<std::bitset<8>(column[1])<<"\n";
  std::cout<<std::bitset<8>(column[2])<<"\n";
  std::cout<<std::bitset<8>(column[3])<<"\n";
  std::cout<<std::bitset<8>(column[4])<<"\n";
  std::cout<<std::bitset<8>(column[5])<<"\n";
  std::cout<<std::bitset<8>(column[6])<<"\n";
  std::cout<<std::bitset<8>(column[7])<<"\n";
}*/

