#include <CapSense.h>

/*
 * TapClip -- as it is known  
 * 
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 */

#define THRESHOLD 500
#define SENSITIVITY 30
#define NUMPINS sizeof(sensePins)/sizeof(int)

#define PIN1 8 //LED
#define PIN2 9 //LED

#define BM0 0
#define BM1 1
#define BM2 2

#define START  0xfe
#define END    0xdc
#define VAL    0x81
  


// CapSense pins
int sensePins[] = { A2, A0, A4 };
int loadPins[] = { A3, A1, A5 };

// Bit values of clips, to bitshift things correctly for serial
int touchClips[] = { BM0, BM1, BM2 };

CapSense   cs_1 = CapSense(A3,A2);        
CapSense   cs_2 = CapSense(A1,A0);       
CapSense   cs_3 = CapSense(A5,A4);      

void setup() {

    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    Serial.begin(9600);

}

void loop() {
    uint8_t i;
    
    byte touchMask = 0x00;
    byte actionMask = VAL;

    long totalVal[NUMPINS];


    long totalVal_prev[NUMPINS];

    long start = millis();

    totalVal[0] =  cs_1.capSense(SENSITIVITY);
    totalVal[1] =  cs_2.capSense(SENSITIVITY);
    totalVal[2] =  cs_3.capSense(SENSITIVITY);


    for(i = 0; i < NUMPINS; i++ ) {

        if(totalVal[i] > THRESHOLD) {

            touchMask |= 1 << touchClips[i];
            digitalWrite(PIN1, HIGH);

        }
    
    }

   
    Serial.write(touchMask);
    Serial.flush();
 

    delay(100);

}
