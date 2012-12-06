#include <CapSense.h>

/*
 * TapClip -- as it is known  
 * 
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 */

#define THRESHOLD 900
#define SENSITIVITY 100
#define NUMPINS sizeof(sP)/sizeof(int)

#define PIN1 8 //LED
#define PIN2 9 //LED

#define BM0 1
#define BM1 0
#define BM2 2
#define BM3 3
#define BM4 4


#define START  0x33
#define END    0x0A 

#define VALUE 0xcc
  


// CapSense pins
int sP[] = { A2, A0, A4, 3, 5 };
int lP[] = { A3, A1, A5, 4,  6 };

// Bit values of clips, to bitshift things correctly for serial
int touchClips[] = { BM0, BM1, BM2, BM3, BM4 };

CapSense   cs_1 = CapSense(lP[0], sP[0]);        
CapSense   cs_2 = CapSense(lP[1], sP[1]);       
CapSense   cs_3 = CapSense(lP[2], sP[2]);      
CapSense   cs_4 = CapSense(lP[3], sP[3]);      
CapSense   cs_5 = CapSense(lP[4], sP[4]);      

void setup() {

    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    Serial.begin(9600);

}

byte prevMask;

void setAsInput(int l, int s) {
  pinMode(l, INPUT);
  pinMode(s, INPUT);  
}

void loop() {
  
    uint8_t i;
    
    byte serialBuffer[7];
    
    byte touchMask = 0x00;  
    byte valueMask[NUMPINS];

    long totalVal[NUMPINS];

    pinMode(lP[0], OUTPUT);
    totalVal[0] =  cs_1.capSense(SENSITIVITY);
    setAsInput(lP[0], sP[0]);
    //Serial.println("0");
    //Serial.println(totalVal[0]);

    pinMode(lP[1], OUTPUT);
    totalVal[1] =  cs_2.capSense(SENSITIVITY);
    setAsInput(lP[1], sP[1]);
    //Serial.println("1");
    //Serial.println(totalVal[1]);

    pinMode(lP[2], OUTPUT);
    totalVal[2] =  cs_3.capSense(SENSITIVITY);
    setAsInput(lP[2], sP[2]);
    //Serial.println("2");
    //Serial.println(totalVal[2]);

    pinMode(lP[3], OUTPUT);
    totalVal[3] =  cs_4.capSense(SENSITIVITY);
    setAsInput(lP[3], sP[3]);
    //Serial.println("4");
    //Serial.println(totalVal[3]);

    pinMode(lP[4], OUTPUT);
    totalVal[4] =  cs_5.capSense(SENSITIVITY);
    setAsInput(lP[4], sP[4]);
    //Serial.println("4");
    //Serial.println(totalVal[4]);

    // Get the current state of the pins
    for(i = 0; i < NUMPINS; i++ ) {
      
        if(totalVal[i] > THRESHOLD) {
            touchMask |= 1 << touchClips[i];
            digitalWrite(PIN2, HIGH);
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
    // serialBuffer[6] = checksum
    serialBuffer[7] = 0x0a;
    
    Serial.write(serialBuffer, 7);
    Serial.flush();
 
    prevMask = touchMask;
    
    delay(20);
    digitalWrite(PIN2, LOW);

}
