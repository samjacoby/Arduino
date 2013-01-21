#include <stdint.h>
#include <avr/io.h>

#include "sinetable.h"

// Variable to hold the next sample that will be output to the speaker
// static volatile uint8_t next_sample;
static volatile uint8_t next_sample;

// Flag indicating whether the next sample has been computed.
// static volatile uint8_t synth_ready;
static volatile uint8_t synth_ready;

void synth_init(void) {
    TCCR0A = (1 << WGM01); //CTC
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = 1 << OCIE0A;
    OCR0A = 127;
}

ISR(TIMER0_COMPA_vect) {
    audio_output(next_sample);
    synth_ready = 0;

}




void synth_generate(void) {

    uint16_t carrier_inc;
    uint16_t carrier_pos;

    uint16_t modulator_inc;
    uint16_t modulator_pos;

    uint16_t mod_ratio_numerator;
    uint16_t mod_ratio_denominator; 

    uint8_t cpos;
    uint8_t mpos;

    uint8_t modulation;

    if(synth_ready) return

    carrier_inc = 127;
    mod_ratio_numerator = 1;
    mod_ratio_denominator = 2;

    modulator_inc = carrier_inc * mod_ratio_numerator / mod_ratio_denominator;

    modulator_pos = 0;
    carrier_pos = 0;

    cpos = 0;
    mpos = 0;

    modulator_pos += modulator_inc;
    mpos += modulator_pos & SINETABLE_MASK;
    modulation = pgm_read_byte(sinetable[&mpos]);

    carrier_pos += carrier_inc;
    cpos += carrier_inc + modulation;

    sample = pgm_read_byte(sinetable[&cpos]);

    synth_ready = 1;
}
