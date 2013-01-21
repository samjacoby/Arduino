#include <stdio.h>
#include <avr/io.h>
#include <avr/interript.h>
#include <CapSense.h>

#include "audio.h"
#include "sinetable.h"

CapSense clip_one = CapSense(9, 10);
CapSense clip_two = CapSense(4, 12);
CapSense clip_three = CapSense(13, 5);

uint8_t next_sample = 0;

void setup() 
{
   cli();
   audio_init();
   timer0_init();
   sei();

   Serial.begin(115200);
}

long total1, total2, total3;
char * data_str = (char*) malloc(18 * sizeof(char));
void loop() {

    total1 =  clip_one.capSense(5);
    total2 =  clip_two.capSense(5);
    total3 =  clip_three.capSense(5);

    sprintf(data_str, "%lu, %lu, %lu", total1, total2, total3);
    
    Serial.println(data_str);
    delay(10);

}

ISR(TIMER1_COMPA_vect) {
    audio_output(next_sample);
}

/*void timer1_init() {/*{{{*/

    DDRD |= (1 << PIND0);
    TCCR1A = (1 << WGM11) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << CS10) ;   
    TIMSK1 = (1 << OCIE1A);
    
}*/ /*}}}*/
void timer0_init() {/*{{{*/
    DDRF = (1 << PINF0);
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = 1 << OCIE0A;
    OCR0A = 127;
}

ISR(TIMER0_COMPA_vect) {
        PORTF ^= (1 << PINF0);/*}}}*/
}
