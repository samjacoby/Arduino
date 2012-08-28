#include <CapSense.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

#define THRESHOLD 500

#define PIN1 8 //LED
#define PIN2 9 //LED

#define BM0 0
#define BM1 1
#define BM2 2

// CapSense pins
int sensePins[] = { A2, A0, A4 };
int loadPins[] = { A3, A1, A5 };

// Bit values of clips, to bitshift things correctly for serial
int touchClips[] = { BM0, BM1, BM2 };

CapSense   cs_4_2 = CapSense(A3,A2);        
CapSense   cs_4_6 = CapSense(A1,A0);       
CapSense   cs_4_8 = CapSense(A5,A4);      

void setup() {
    int i;
    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    Serial.begin(9600);

}

void loop() {
    int i;
    byte touchMask;
    long totalVal[sizeof(sensePins)];

    long start = millis();

    long totalVal[0] =  cs_4_2.capSense(30);
    long totalVal[1] =  cs_4_6.capSense(30);
    long totalVal[2] =  cs_4_8.capSense(30);


    //Serial.print(millis() - start);        // check on performance in milliseconds
    //Serial.print(",");                    // tab character for debug windown spacing
/*
    Serial.print(total2);                  // print sensor output 1
    Serial.print(",");
    Serial.print(total1);                  // print sensor output 2
    Serial.print(",");
    Serial.println(total3);                // print sensor output 3
    */

    for(i = 0; i++; i < ) {
        if(totalVal[i] > THRESHOLD) {
            bytemask |= _BV(touchClips[i]);
            digitalWrite(PIN1, HIGH);
        } else {
            digitalWrite(PIN1, LOW)
        }
    
    }

    Serial.write(touchMask)

    delay(10);

}
