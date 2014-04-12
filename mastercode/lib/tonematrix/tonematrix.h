#ifndef TONE_MATRIX_H
#define TONE_MATRIX_H
#include<Arduino.h>
enum mode{SPIRAL,PULSE};
//typedef unsigned char byte;
class toneMatrix
{
private:	
	byte column[8];
	static const byte tree[2][96];
	static const byte pad[2][32];
	mode light_mode;
	void placement(const byte * place, byte isOn);
public:
	toneMatrix();
	void setMode(mode m);
	void print(Stream& s);
	//void print(void);

	void fromTree(byte branch,byte address,byte state);
	void fromPad(byte a,byte b, byte c, byte d);
	const byte * getBytes(void);
};



#endif
