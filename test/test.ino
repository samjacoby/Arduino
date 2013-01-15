//#include <CapSense.h>

#define LED1 A4 
#define LED2 A5 

void setup() {

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //pcm_init();

    DDRD = (1 << PIND0);
}

void pcm_init() {

    TCCR0A =  (1 << WGM01) |  (1 << WGM00) | (1 << COM0B1);

    // set prescaling on timer
    TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);

    // set interrupts (use OCR0A for CTC)
    //TIMSK0 = (1 << OCIE0B);
    //
    OCR0A = 255;
    OCR0B = 32;


}

void loop() {

    for(int j=1; j<10; j++) {
        for(int i=0; i<255; i++) {
            PORTD ^= (1 << PIND0);        
            delayMicroseconds(i*j);
        }
    }


}
