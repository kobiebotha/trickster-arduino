#include <CapacitiveSensor.h>

CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,16);        

#define debounceThreshold 5;

//We run in touch mode since proximity sensing isn't working
int proximityMode = 0;

long max = 0;

int dbncCount = 0;

boolean sensorOn = false;

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
}

void proximityLoop() {
    //TODO: Implement once the sensor works
}

boolean toggle(boolean state) {
    if (state == false) {
        return true;
    }
    return false;
}

void touchLoop() {
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);

    if (total1 > max) {
        max = total1;
    }

    if (total1 >= max*0.85) {
        dbncCount++;
        sensorOn = toggle(sensorOn);
    }

    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);                  // print sensor output 1

    Serial.print("\t sensr=");
    Serial.print(sensorOn);

    Serial.print("\t max=");
    
    Serial.println(max);
    //Serial.print("\t");
    //Serial.println(total1);
    delay(10);                             // arbitrary delay to limit data to serial port 
}

void loop()                    
{
    if (proximityMode) {
         proximityLoop();
    } 
    else { 
        touchLoop();
    }
}