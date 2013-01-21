#include <avr/io.h>
#include "audio.h"

/* At a system clock of 16Mhz and a signal bit depth
 * of 8, the PWM frequency will be running at 
 * 62.5 KHz. The duty cycle is varied by the value of
 * OCR1A.
 */
void audio_init(void) {

    DDRD |= (1 << PIND0);
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS10);   
    TIMSK1 = (1 << OCIE1A);

    OCR1A = 0;

}


void audio_output(uint8_t next_sample) {
    // TODO: essentially, this should only set OCR1A. 
    OCR1A = next_sample; 
}
