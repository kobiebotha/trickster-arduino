#include <Keypad.h>

#define latchPin 30
#define bClockPin 27
#define bDataPin 23

#define slatchPin 31
#define sbClockPin 29
#define sbDataPin 25

extern uint32_t slaveState;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
{'0','1','2','3'},
{'4','5','6','7'},
{'8','9','A','B'},
{'C','D','E','F'}
};
byte rowPins[ROWS] = {42, 40, 38, 36}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {34, 39, 41, 43}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//looping variables
byte ledRow;
byte ledBit;

//storage variable
byte dataToSend;
byte sdataToSend;



void LEDDisplaySetup() {
  //set pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(bClockPin, OUTPUT);
  pinMode(bDataPin, OUTPUT);
  
  pinMode(slatchPin, OUTPUT);
  pinMode(sbClockPin, OUTPUT);
  pinMode(sbDataPin, OUTPUT);
}

void LEDDisplayLoop() {
  int keyPress;
  for (ledRow=0;ledRow<4;ledRow++){  
    dataToSend = (~(1<<ledRow+4)& 0xF0) | (byte(slaveState>>(ledRow*8))&0x0F);
    //sdataToSend = ~((1<<ledRow+4) | (1<<ledRow));
    digitalWrite(latchPin, LOW);
    //digitalWrite(slatchPin, LOW);   
    for (ledBit=0;ledBit<8;ledBit++){
      digitalWrite(bClockPin,LOW);
      //digitalWrite(sbClockPin,LOW);
      digitalWrite(bDataPin,((dataToSend>>ledBit)&1));
      //digitalWrite(sbDataPin,((sdataToSend>>(7-ledBit))&1));
      digitalWrite(bClockPin,HIGH);
      //digitalWrite(sbClockPin,HIGH);
    }
    digitalWrite(latchPin, HIGH);
    //digitalWrite(slatchPin, HIGH);    
  }
  
  
  if (kpd.getKeys())
  {
      for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
      {
          if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
          {
              switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                  case PRESSED:
                    keyPress = LEDhex2num(kpd.key[i].kchar);
                    if (bitRead(slaveState,keyPress+((keyPress/4)*4))){
                      bitClear(slaveState,keyPress+((keyPress/4)*4));
                    } else {
                      bitSet(slaveState,keyPress+((keyPress/4)*4));
                    }
                    break;
                  case HOLD:
                    break;
                  case RELEASED:
                    break;
                  case IDLE:
                    break;
              }
          }
      }
  }  
  
}


int LEDhex2num(byte x){

  int result;

  if (x>=48 && x<=57){
    result=x-48;  
  }
  else if (x>=65 && x<=70){
    result=x-55;    
  }
  return result;  
}
