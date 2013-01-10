#include <CapSense.h>


/*
 * TapClip -- as it is known  
 * 
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 */
//#define DEBUG

#define THRESHOLD 900
#define SENSITIVITY 100
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

const int NUMSAMPLES = 5;

// CapSense pins
int sP[] = { A0, A2, A4, 3, 5 };
int lP[] = { A1, A3, A5, 4,  6 };

CapSense cPins [] = {CapSense(lP[0], sP[0]), CapSense(lP[1], sP[1]),CapSense(lP[2], sP[2]),CapSense(lP[3], sP[3]),CapSense(lP[4], sP[4])}; 
long cPinsMax[] = {0,0,0,0,0};
long totalVal[] = {0,0,0,0,0};
long lastRead[NUMPINS][NUMSAMPLES];

// Bit values of clips, to bitshift things correctly for the touchmask 
int touchClips[] = { BM0, BM1, BM2, BM3, BM4 };


void setup() {

    // LEDs
    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    Serial.begin(57600);
    // initialize the sucker
    for(int i = 0; i < NUMPINS; i++) {
        for(int j = 0; j < NUMPINS; j++) {
            lastRead[i][j] = 0;
        }
    }


}

byte prevMask;

int inByte;
long sensorRead;
uint8_t j = 0;
void loop() {

    uint8_t i = 0;

    byte serialBuffer[8];
    byte touchMask = 0x00;  

    
    long removeVal;
    long MAX = 1024;
    long movingAvg[5];

    char buffer[40];


    // reset calibration on all pins
    if(Serial.available() > 0) {
        inByte = Serial.read();
        for(i=0;i < NUMPINS;i++) {
            digitalWrite(PIN2, HIGH);
            cPins[i].reset_CS_AutoCal();
            digitalWrite(PIN2, LOW);
        }
    }

    // read the actual sensors and keep your hat on
    int n;
    for(i = 0; i < NUMPINS; i++) {
        pinMode(lP[i], OUTPUT);

        sensorRead = cPins[i].capSense(SENSITIVITY);
        lastRead[i][j] = sensorRead; // store last value in the right place
        totalVal[i] += sensorRead;  // keep running total
        cPinsMax[i] = (sensorRead > cPinsMax[i]) ? sensorRead : cPinsMax[i]; // keep track of the maximum reading on a given pin
        totalVal[i] -= lastRead[i][(j+1)%NUMSAMPLES]; // remove value from running total

        setAsInput(lP[i], sP[i]);   // library doesn't. 
    }

    j = (j + 1) % NUMSAMPLES; // get the last value

    // grab moving average of each pin
    for(i = 0; i < NUMPINS; i++) {
        movingAvg[i] = totalVal[i] / (NUMSAMPLES); 
    }
    #ifdef DEBUG
    n=sprintf(buffer, "%ld, %ld, %ld, %ld, %ld", totalVal[0], totalVal[1], totalVal[2], totalVal[3], totalVal[4]);
    Serial.println(buffer);
    #endif




    // Get the current state of the pins

    for(i = 0; i < NUMPINS; i++ ) {
      
        if(movingAvg[i] > THRESHOLD) {
            touchMask |= 1 << touchClips[i];
            digitalWrite(PIN2, HIGH);
        }
        
        // Populate buffer and do mapping
        serialBuffer[i + 2] = map(movingAvg[i], 0, cPinsMax[i], 0, 255);
    }

    #ifdef DEBUG
    n=sprintf(buffer, "%d, %d, %d, %d, %d", serialBuffer[0+2], serialBuffer[1+2], serialBuffer[2+2], serialBuffer[3+2], serialBuffer[4+2]);
    Serial.println(buffer);
    #endif
    
    // Set start and end bytes
    serialBuffer[0] = START;
    serialBuffer[7] = END;

    // Add touchmask
    serialBuffer[1] = touchMask;
    
    #ifndef DEBUG
    Serial.write(serialBuffer, 8);
    Serial.flush();
    #endif
 
    prevMask = touchMask;
    
    delay(20);
    digitalWrite(PIN2, LOW);

}

void writeMessage(byte msg[]) {


}

void setAsInput(int l, int s) {
  pinMode(l, INPUT);
  pinMode(s, INPUT);  
}


