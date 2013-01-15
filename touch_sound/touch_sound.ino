#include <CapSense.h>

#define LED1 PINF1 
#define LED2 PINF0 

void setup() {

    DDRF |= (1 << PINF1) | (1 << PINF0);
    DDRD = (1 << PIND0);
    timer0_init();
    timer1_init();
    sei();
}

void timer0_init() {
    TCCR0A = 1 << WGM01; 
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = 1 << OCIE0B;
    OCR0A = 48;

}

void timer1_init() {
    TCCR1B = (1 << CS12) | (1 << CS10);
    TIMSK1 = (1 << TOIE1);
}


int i, j;
void loop() {
}

ISR(TIMER0_COMPB_vect) {
    PORTD ^= (1 << PIND0);
    OCR0A++;
}

ISR(TIMER1_OVF_vect) {
    PORTF ^= (1 << PINF0) | (1 << PINF1);
}
