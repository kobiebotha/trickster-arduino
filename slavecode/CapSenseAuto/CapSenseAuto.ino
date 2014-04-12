//TODO: implement state changing that requires a "low" reading to change state
//TODO: reset max since it fucks out to insanely high values sometimes
//TODO: define "low" and "high" thresholds (move average)


#include <CapacitiveSensor.h>

CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,16);        

#define debounceThreshold 5

//We run in touch mode since proximity sensing isn't working
int proximityMode = 0;

// the highest value ever read from the sensor
long max = 0;

// debounce counter
int dbncCount = 0;

// sensor on or off - reported to master
boolean sensorOn = false;

// length of moving average
#define MOV_AVE_LEN 2

// moving average values
long maSamples[MOV_AVE_LEN];

// the moving average
long ma;

// the moving average counter 
long maCount = 0;

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(115200);
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

double average(long array[MOV_AVE_LEN]) {
    int i = 0;
    double result = 0;
    for (i = 0; i < MOV_AVE_LEN; i++) {
        result += array[i];
    }
    return result / MOV_AVE_LEN;
}

void updateMovingAverage(long newReading) {
    maSamples[maCount] = newReading;
}

void touchLoop() {
    long start = millis();
    //long total1 = cs_4_2.capacitiveSensor(10);
    int i = 0;

    for (i = 0; i < MOV_AVE_LEN; i++) {
        maSamples[i] = cs_4_2.capacitiveSensor(5);;
    }
    double total1 = average(maSamples);

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