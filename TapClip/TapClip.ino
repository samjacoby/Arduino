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

#define PRESS  0x33
#define END    0xff
#define VALUE    0xcc
  


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

byte prevMask;

void loop() {
  
    uint8_t i;
    
    byte serialBuffer[4];
    
    byte touchMask = 0x00;  
    byte valueMask[NUMPINS];
    byte actionMask;

    long totalVal[NUMPINS];

    totalVal[0] =  cs_1.capSense(SENSITIVITY);
    totalVal[1] =  cs_2.capSense(SENSITIVITY);
    totalVal[2] =  cs_3.capSense(SENSITIVITY);


    // Get the current state of the pins
    for(i = 0; i < NUMPINS; i++ ) {
      
        if(totalVal[i] > THRESHOLD) {
            touchMask |= 1 << touchClips[i];
            digitalWrite(PIN1, HIGH);
        }
        
        serialBuffer[i + 1] = map(0, 1023, 0, 255, totalVal[i]);
    }
    
    if(prevMask != touchMask) {
      serialBuffer[0] = PRESS;
      serialBuffer[1] = touchMask;
    } else {
      serialBuffer[0] = VALUE;
    }
    
    // endbyte
    serialBuffer[4] = 0xff;
    
    Serial.write(serialBuffer, 5);
    Serial.flush();
 
    prevMask = touchMask;

    delay(20);

}
