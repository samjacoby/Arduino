//                  +-\/-+
// Ain0 RESET(D 5) PB5   1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2)  Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0
//                  +----+

#define TIMEOUT 10000

byte touchPins[] ={ 3, 4, 5 };//, PB5};
byte ledPins[] = { 0, 1, 2 };

int calibration[sizeof(touchPins)];

void setup()
{
  
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  // Need a minute to get fingers outt've the way. 
  delay(1000);
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
    
      digitalWrite(ledPins[i], HIGH);
      last = millis();
    
      do {
        delay(2);
        n = chargeTimeR(touchPins[i]);
        if(millis() - last > TIMEOUT) {
          calibrate();
          break;
        }
      } while (n > calibration[i]);     
      digitalWrite(ledPins[i], LOW);
    } else {
      digitalWrite(ledPins[i], LOW);
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


