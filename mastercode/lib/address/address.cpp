#include"address.h"

void address()
{
  Serial.begin(9600);
    Serial.println("Send number to set my address > 0");
    Serial.println("0 to use internal address");
    byte time=0;
    while(!Serial.available()&&time<50)
    {
      delay(100);
      time=time+1;
      //Serial.println(time);
    }
    byte c=Serial.parseInt();
    if(c==0)
    {
      Serial.println("Using internal address");
      Serial.println(EEPROM.read(0)+1);
    }
    else
    {
      Serial.println("Writing address to EEPROM");
      EEPROM.write(0,c-1);
    }
  Serial.end();
}
