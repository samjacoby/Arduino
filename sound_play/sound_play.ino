#include <CapSense.h>

int i;
long mappedSensorRead, sensorRead, maxSensorRead, minSensorRead;

//CapSense c = CapSense(A0, A1);
//

CapSense c = CapSense(PINC7, PINC6);
void setup() {
    
    TCCR0A = (1 << COM0A0) | (1 << WGM01);
    OCR0A = 127;
    TIMSK0 = (1 << OCIE0A);
    DDRD = (1 << 0);
}



void loop() {

    sensorRead = c.capSense(1);
    if(sensorRead > maxSensorRead) {
        maxSensorRead = sensorRead;
    }
    if(sensorRead < minSensorRead) {
        minSensorRead = sensorRead;
    }

    mappedSensorRead = map(sensorRead, minSensorRead, maxSensorRead, 60, 255);
    i = (int) mappedSensorRead;

}

ISR(TIMER0_COMPA_vect) {
    OCR0A = i;
    PORTD ^= (1 << 0);
}
