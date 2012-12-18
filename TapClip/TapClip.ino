#include <CapSense.h>

/*
 * TapClip -- as it is known  
 * 
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 */

#define THRESHOLD 900
#define SENSITIVITY 30
#define NUMPINS sizeof(sP)/sizeof(int)

#define PIN1 8 //LED
#define PIN2 9 //LED

#define BM0 0
#define BM1 1
#define BM2 2
#define BM3 3
#define BM4 4


#define START  0xFE
#define END    0x71 

#define VALUE 0xcc

// CapSense pins
int sP[] = { A0, A2, A4, 3, 5 };
int lP[] = { A1, A3, A5, 4,  6 };

CapSense cPins [] = {CapSense(lP[0], sP[0]), CapSense(lP[1], sP[1]),CapSense(lP[2], sP[2]),CapSense(lP[3], sP[3]),CapSense(lP[4], sP[4])}; 

// Bit values of clips, to bitshift things correctly for serial
int touchClips[] = { BM0, BM1, BM2, BM3, BM4 };
CapSense   cs_1 = CapSense(lP[0], sP[0]);        
CapSense   cs_2 = CapSense(lP[1], sP[1]);       
CapSense   cs_3 = CapSense(lP[2], sP[2]);      
CapSense   cs_4 = CapSense(lP[3], sP[3]);      
CapSense   cs_5 = CapSense(lP[4], sP[4]);      
/*
cPins[0] = cs_1;
cPins[1] = cs_2;
cPins[2] = cs_3;
cPins[3] = cs_4;
cPins[4] = cs_5;
*/

void setup() {

    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    Serial.begin(57600);

}

byte prevMask;

void setAsInput(int l, int s) {
  pinMode(l, INPUT);
  pinMode(s, INPUT);  
}

int inByte;
void loop() {

    uint8_t i;

    byte serialBuffer[8];
    byte touchMask = 0x00;  

    long totalVal[NUMPINS];
    long MAX = 1024;

    char buffer[40];

    // reset calibration on all pins
    if(Serial.available() > 0) {
        inByte = Serial.read();
        for(i=0;i<5;i++) {
            digitalWrite(PIN2, HIGH);
            cPins[i].reset_CS_AutoCal();
            digitalWrite(PIN2, LOW);
        }
    }
    
    // Scan across all pins in sequence
    for( i = 0; i < NUMPINS; i++) {
        pinMode(lP[i], OUTPUT);
        totalVal[i] = cPins[i].capSense(SENSITIVITY);
    
        setAsInput(lP[i], sP[i]); 
        //Serial.println(totalVal[i]);
    
    }
    int n=sprintf(buffer, "%ld, %ld, %ld, %ld, %ld", totalVal[0], totalVal[1], totalVal[2], totalVal[3], totalVal[4]);
    //Serial.println(buffer);

    // Get the current state of the pins
    for(i = 0; i < NUMPINS; i++ ) {
      
        if(totalVal[i] > THRESHOLD) {
            touchMask |= 1 << touchClips[i];
            digitalWrite(PIN2, HIGH);
        }
        
        // Populate buffer
        serialBuffer[i + 2] = map(totalVal[i], 0, MAX, 0, 255);
    }
    n=sprintf(buffer, "%d, %d, %d, %d, %d", serialBuffer[0+2], serialBuffer[1+2], serialBuffer[2+2], serialBuffer[3+2], serialBuffer[4+2]);
    //Serial.println(buffer);
    
    // Set start and end bytes
    serialBuffer[0] = START;
    serialBuffer[7] = END;

    // Add touchmask
    serialBuffer[1] = touchMask;
    
    Serial.write(serialBuffer, 8);
    Serial.flush();
 
    prevMask = touchMask;
    
    delay(20);
    digitalWrite(PIN2, LOW);

}
