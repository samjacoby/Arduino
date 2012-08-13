//                        attiny85
//                   reset -+---+- power
// (on while touching) pb3 -+*  +- pb2 (toggled by touch)
//       (touch input) pb4 -+   +- pb1 (fading while touching)
//                  ground -+---+- pb0 (fading always)

byte touchPins[] = { PB2, PB3, PB4, PB1 };//, PB5};
byte ledPins[] = { 
  PB0};

int calibration[sizeof(touchPins)];

void setup()
{
 
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  initialize();
  calibrate();

}

void initialize() {

  byte i, j; 
  
  for(i = 0; i < sizeof(touchPins); i++) {
    pinMode(touchPins[i], INPUT);  
  }

  for(byte i = 0; i < sizeof(ledPins); i++) {
    pinMode(ledPins[i], OUTPUT);  
  }
}

void calibrate() {
  byte i, j;

  int max = 0;
  
  for(j = 0; j < sizeof(touchPins); j++) {
    
    for (i = 0; i < 8; i++) {
      
      calibration[j] += chargeTimeR(touchPins[j]);
      delay(200);
    }
  
  calibration[j] = (calibration[j] + 4 * sizeof(touchPins)) / (8 * sizeof(touchPins));
  }
  
  
  digitalWrite(ledPins[0], HIGH);
  delay(500);
  digitalWrite(ledPins[0], LOW);
  delay(500);
}

void loop()
{
  byte i, n;
  for(i = 0; i < sizeof(touchPins); i++) {
    n = chargeTimeR(touchPins[i]);
    
    if (n > calibration[i]) {
      digitalWrite(ledPins[0], HIGH);
      do {
        delayMicroseconds(500);
        n = chargeTimeR(touchPins[i]);
      } while (n > calibration[i]);     
      digitalWrite(ledPins[0], LOW);
    } else {
      digitalWrite(ledPins[0], LOW);
    }
    
    delayMicroseconds(500);
  }
}

int chargeTimeR(byte pin) {
  int i, time;
  time = 0;
  for(i = 0; i < 4; i++) {
    time += chargeTime(pin);
    delay(200);
  }
  return time;
}


byte chargeTime(byte pin)
{
  byte mask = (1 << pin);
  byte i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}


