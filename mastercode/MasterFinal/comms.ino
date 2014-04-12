#include <LCDshield.h>
#include <LEDmatrix.h>
#include <keypadbuttons.h>
#include <tonematrix.h>

toneMatrix mine;
byte serialData[16];
byte analog[5];
extern int wang;
extern int modeIndex;
extern byte row1;
extern byte row2;
extern byte row3;
extern byte row4;

void matrixModes()
{
  int i=0;
  
  const byte *tempR1;
  int analogI[5];
  
  
  mine.fromPad(row1,row2,row3,row4);
  
  tempR1=mine.getBytes();
  
  serialData[0]=1;
  serialData[1]=tempR1[0];
  serialData[2]=1;
  serialData[3]=tempR1[1];
  serialData[4]=1;
  serialData[5]=tempR1[2];
  serialData[6]=1;
  serialData[7]=tempR1[3];
  serialData[8]=1;
  serialData[9]=tempR1[4];
  serialData[10]=1;
  serialData[11]=tempR1[5];
  serialData[12]=1;
  serialData[13]=tempR1[6];
  serialData[14]=1;
  serialData[15]=tempR1[7];
  serialData[16]=1;
  
  analog[0]=analogRead(12);
  analog[1]=analogRead(13);
  analog[2]=analogRead(14);
  analog[3]=analogRead(15); 
      
        for(i=1;i<16;i=i+2)
        {        
          if(serialData[i]==0)
          {
          serialData[i-1]=0x02;
          serialData[i]=1;
          }
        }              
        
      Serial.write(0);        
      Serial.write(1);      
      Serial.write(1);     
      for(i=0;i<16;i++)
      {
      Serial.write(serialData[i]);
      }
      Serial.write(analog[0]);
      Serial.write(analog[1]);
      Serial.write(analog[2]);
      Serial.write(analog[3]);
     
      
      for(i=0;i<16;i=i+2)
        {        
          if(serialData[i]==0x02)
          {
          serialData[i+1]=0;          
          }
        }
        
}
