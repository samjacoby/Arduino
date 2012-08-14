//                        attiny85
//                   reset -+---+- power
// (on while touching) pb3 -+*  +- pb2 (toggled by touch)
//       (touch input) pb4 -+   +- pb1 (fading while touching)
//                  ground -+---+- pb0 (fading always)

#define TIMEOUT 10000

byte touchPins[] ={ 1, 2, 3, 4, 5};//, PB5};
byte ledPins[] = { 0 };

int calibration[sizeof(touchPins)];

void setup()
{
  
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  // Need a minute to get fingers outt've the way. 
  delay(5000);
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
  
  digitalWrite(ledPins[0], HIGH);
  delay(500);
  digitalWrite(ledPins[0], LOW);
  delay(500);
  
  for(j = 0; j < sizeof(touchPins); j++) {

    for (i = 0; i < 8; i++) {
      digitalWrite(ledPins[0], LOW);
      calibration[j] += chargeTimeR(touchPins[j]);
      delay(5);
      digitalWrite(ledPins[0], HIGH);

    }
  
    calibration[j] = (calibration[j] + 4) / 8;
  }
  
  digitalWrite(ledPins[0], HIGH);
  delay(500);
  digitalWrite(ledPins[0], LOW);
  delay(500);
}

void loop()
{
  long last;
  byte i, n;
  for(i = 0; i < sizeof(touchPins); i++) {
    
    n = chargeTimeR(touchPins[i]);
    
    if (n > calibration[i]) {
    
      digitalWrite(ledPins[0], HIGH);
      last = millis();
    
      do {
        delay(2);
        n = chargeTimeR(touchPins[i]);
        if(millis() - last > TIMEOUT) {
          calibrate();
          break;
        }
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
  for(i = 0; i < 24; i++) {
    time += chargeTime(pin);
    delayMicroseconds(500);
  }
  return time;
}


byte chargeTime(byte pin)
{
  byte mask, i;
  mask = digitalPinToBitMask(pin);
  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}


