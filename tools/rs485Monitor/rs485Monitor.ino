
#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

#define mybaud           19200


SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX


int byteReceived;
int byteSend;
int i;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(mybaud);
  Serial.println("SerialRemote");  // Can be ignored
  
  pinMode(Pin13LED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);  
  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver
  
  // Start the software serial port, to another device
  RS485Serial.begin(mybaud);   // set the data rate
  i = 0;
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{

  //Copy input data to output  
  if (RS485Serial.available()) 
  {
    i = i+1;
    if ( i >= 15) {
       Serial.println(" ");
       i = 0;
    }
    byteSend = RS485Serial.read();   // Read the byte

    Serial.write(byteSend); // Send the byte back

  }// End If RS485SerialAvailable
  
}//--(end main loop )---

