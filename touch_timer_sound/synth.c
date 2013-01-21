
void generate(void) {

    uint16_t carrier_inc;
    uint16_t carrier_pos;

    uint16_t modulator_inc;
    uint16_t modulator_pos;

    uint16_t mod_ratio_numerator;
    uint16_t mod_ratio_denominator; 

    uint8_t cpos;
    uint8_t mpos;

    uint8_t modulation;


    carrier_inc = 127;
    mod_ratio_numerator = 1;
    mod_ratio_denominator = 2;
    modulator_inc = carrier_inc * (mod_ratio_numerator) / (mod_ratio_denominator);

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









    pgm_read_byte(sinetable[&cpos])



}
