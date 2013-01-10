#include <CapSense.h>
//Analog out
//by Amanda Ghassaei
//http://www.instructables.com/id/Arduino-Audio-Output/
//Sept 2012

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

CapSense c = CapSense(A0, A1);
void setup(){
  //set digital pins 0-7 as outputs
    for (int i=0;i<8;i++){
        pinMode(i,OUTPUT);
    }
}

long sensorRead;
long mappedSensorRead;
int i;
void loop(){
    /*
    for(int i=0; i<255; i++) {
        PORTD = i;//send (0/255)*5 = 0V out DAC
        delayMicroseconds(10);
        
    }
    */
    sensorRead = c.capSense(30);
    mappedSensorRead = map(sensorRead, 0, 10000, 400,800);
    i = (int) mappedSensorRead;
    

/*
    for(int i=400; i<800; i++) {
        PORTD = 0;
        delayMicroseconds(i);
        PORTD = 255;
        delayMicroseconds(i);
    }
    for(int i=800; i>400; i--) {
        PORTD = 0;
        delayMicroseconds(i);
        PORTD = 255;
        delayMicroseconds(i);
    }
    */
        PORTD = 0;
        delayMicroseconds(i);
        PORTD = 255;
        delayMicroseconds(i);
}

