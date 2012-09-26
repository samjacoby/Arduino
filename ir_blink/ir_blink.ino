#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Define relevant registers in the slave device
// #include "register_map.h"
// Version information, for late queries and such.
// #include "version.h"

#define IR_FREQ 38000L
#define FLICKER 3750

volatile uint8_t ir_count;
volatile uint16_t sound;

boolean dataRead;

void setup(void) {

    // Serial Init
    Serial.begin(115200);

    // Set chip to run at 16MHz
    CLKPR = (1 << CLKPCE); 
    CLKPR = 0;

    // CONFIGURE TIMER0
    // Configure 38Khz
    // Run the timer w/ a prescale
    TCCR0B = 1 << CS01;

    OCR0A = 25;     // regulates frequency. some number.
    OCR0B = 12;     // doesn't really do anything now... 

    TCCR0A |= (1 << WGM01 ) | (1 << COM0B0);

    // Enable interrupts in the right places
    //TIMSK0 = 1 << OCIE0B;

    // CONFIGURE TIMER1
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);   // CTC + Clock scaling 
    OCR1A = FLICKER / 64;

    TIMSK1 = (1 << OCIE1A); // Interrupt enable

    DDRB |= 0xff; 

}



ISR(TIMER1_COMPA_vect) {
    static uint8_t count;

    count = (count + 1) & 0b00111111;

    PORTB ^= (1 << 12);
    
    if(count < ir_count) { 
        DDRD |= 1 << 5;
    } else {
        DDRD &= 0;
    }

}


void loop() {
    sei();
    int i;
    for(i = 0; i < 64; i++) {
        ir_count = i;
        _delay_ms(15);
    }
    _delay_ms(220);
    for(i = 63; i >= 0; i--) {
        ir_count = i;
        _delay_ms(15);
    }
    _delay_ms(60);

/*    if(dataRead) {
        ir_count = map(0, 100, 0, 64, 100);      
        dataRead = false;
    }
    */
}
/*
void serialEvent() {

    char* dataString = "";
    while (Serial.available()) {
        char c = (char) Serial.read();
        dataString += c;
        if(c = '\n') {
            dataRead = true; 
            sound = atoi(dataString);
        }
    }

}
*/
