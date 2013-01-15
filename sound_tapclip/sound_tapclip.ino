#include <CapSense.h>
#include <limits.h>
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

long sensorRead, maxSensorRead, minSensorRead;
CapSense c = CapSense(A0, A1);

void setup(){

    sei(); //enable interrupts (default enabled, I think?)
    //TCCR0A |= (1 << COM0A0) | (1 << WGM01); // CTC mode
    TCCR0A |= (1 << WGM01); // CTC mode

    // set prescaling on timer
    TCCR0B |= (1 << CS01) | (1 << CS00); 
    TIMSK0 |= (1 << OCIE0A);

    OCR0A = 255;

    maxSensorRead = 0;
    minSensorRead = INT_MAX;
    //set digital pins 0-7 as outputs
    DDRD = (1 << 6);

}

long mappedSensorRead;
int i;
void loop(){
/*

    sensorRead = c.capSense(1);
    if(sensorRead > maxSensorRead) {
        maxSensorRead = sensorRead;
    }
    if(sensorRead < minSensorRead) {
        minSensorRead = sensorRead;
    }

    mappedSensorRead = map(sensorRead, minSensorRead, maxSensorRead, 0, 255);
    i = (int) mappedSensorRead;
    */
    i = 50;

    
   /* 
    for(int i=0; i<255; i++) {
        PORTD = i;
        delayMicroseconds(10);
    }
   
    
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
        
}

void ISR(TIMER0_COMPA_vect) {
    OCR0A = i;
    PORTD ^= (1 << 6); 
}
