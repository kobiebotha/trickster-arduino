#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
extern byte start;
extern int ledCounter;
int lcd_key     = 0;
int adc_key_in  = 0;
int mode =0;
int colour=1;
int modeIndex =0;
int colourIndex=0;
int j=0;
byte k=0;
byte l=0;
int printlcd;
byte btnPREV=5;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void lcdSetup(void)
{
  lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Sentient Forest"); // print a simple message
}

void lcd_loop()
{
  k++;
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 //lcd.print(millis()/1000);      // display seconds elapsed since power-up
 
 
         // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons
  
 if (mode==0&&printlcd==0)
 {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Sentient Forest");
   lcd.setCursor(0,1);
   lcd.print("Press any button"); 
   printlcd=1;
 }
 
 if (mode==1&&printlcd==0)
 {
   
   lcd.clear();
  
   if (colour==0)
   {
   lcd.setCursor(0,0);   
   lcd.print("->Mode: ");   
   lcd.setCursor(0,1);   
   lcd.print("  Colour: ");
   lcd.setCursor(10,0);
   
     if(modeIndex)
       lcd.print("Spiral");
     else
       lcd.print("Pulse");  
   }
   else
   {
   lcd.setCursor(0,0);   
   lcd.print("  Mode: ");   
   lcd.setCursor(0,1);   
   lcd.print("->Colour: ");
    lcd.setCursor(10,1);
    if(colourIndex==0)
       lcd.print("Red  ");
    else if(colourIndex==1)
       lcd.print("Blue ");          
    else if(colourIndex==2)
       lcd.print("Green");         
    else
       lcd.print("Multi");    
   }
   printlcd=1;
 }
  
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   
   case btnRIGHT:
     {
       
       mode=1;
       printlcd=0;          
       k++;
       j=0;
       if(k<6)
       {
         btnPREV=btnRIGHT;
         break;
       }
       k=0;  
       if(mode==1 && btnPREV==btnRIGHT && colour==0 )     
         {modeIndex=!modeIndex;    
        start=1;
        ledCounter=0;
         } 
       if(mode==1 && btnPREV==btnRIGHT && colour==1)
       { 
         colourIndex++;
         if (colourIndex>3)
         colourIndex=0;
       }       
       btnPREV=btnRIGHT;
       break;
     }
   case btnLEFT:
     {
      
       mode=1;
       printlcd=0;
         
       j=0;
       l++;
       if(l<6)
       {
         btnPREV=btnLEFT;
         break;
       }
       l=0;
       if(mode==1 && btnPREV==btnLEFT && colour==0)
       {
         modeIndex=!modeIndex;
         start=1;
         ledCounter=0;
       }
       if(mode==1 && btnPREV==btnLEFT && colour==1)
       { 
         colourIndex--;
         if (colourIndex<0)
         colourIndex=3;           
       }       
       btnPREV=btnLEFT;
       break;
     }
   case btnUP:
     {
      mode=1;
      printlcd=0;
      j=0;
      if(mode==1 && btnPREV!=btnUP)
        colour=!colour;       
      btnPREV=btnUP;
      break;
     }
   case btnDOWN:
     {
      mode=1;
      printlcd=0;
      j=0;
      if(mode==1 && btnPREV!=btnDOWN)
        colour=!colour;
     
     btnPREV=btnDOWN;
     break;
     }
   case btnSELECT:
     {
     mode=1;
     printlcd=0; 
     btnPREV=btnSELECT;
     break;
     }
   case btnNONE:
     {
         mode=0;
         j++;
         if(j>1000)
         {j=0;
         printlcd=0;
         colour=1;
         }
         btnPREV=btnNONE;
     break;
     }
 }
 
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 
 return btnNONE;  // when all others fail, return this...
}
