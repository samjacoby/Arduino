//                        attiny85
//                   reset   -+---+-  power
//       (touch input) pb3:3 -+*  +- 2:pb2 (fading always)
//       (touch input) pb4:4 -+   +- 1:pb1 (touch something)
//                  ground   -+---+- 0:pb0 (touch something else)

byte touchPins[] ={ 2,3 };//, PB5};
byte ledPins[] = { 5,6,7 };

byte fadepin = 7;
int fadeval = 0, fadestep = 1;

// Calibrate each pin separately
int calibration[sizeof(touchPins)];

void setup()
{

  //Serial.begin(9600);

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

  for(j = 0; j < sizeof(touchPins); j++) {

    for (i = 0; i < 8; i++) {

      calibration[j] += chargeTimeR(touchPins[j]);
      delay(200);
    }

    calibration[j] = (calibration[j] + 4) / 8;
  }

  // Signal calibration complete
  for(i=0;i< 5;i++) {
    digitalWrite(fadepin, HIGH);
    delay(100);
    digitalWrite(fadepin, LOW);
    delay(100);
  }
}

void loop()
{

  analogWrite(fadepin, fadeval);

  if(fadeval == 255) fadestep = -1;
  if(fadeval == 0) fadestep = 1;
  fadeval = fadeval + fadestep;

  byte i, n;
  // Scan all of the touchpins 
  for(i = 0; i < sizeof(touchPins); i++) {
    n = chargeTimeR(touchPins[i]);

    if (n > calibration[i]) {
      digitalWrite(ledPins[i], HIGH);
      do {
        delayMicroseconds(500);
        n = chargeTimeR(touchPins[i]);
      } 
      while (n > calibration[i]);     
      digitalWrite(ledPins[i], LOW);
    } 
    else {
      digitalWrite(ledPins[i], LOW);
    }

    delayMicroseconds(500);
  }
}

int chargeTimeR(byte pin) {
  int i, time;
  time = 0;
  for(i = 0; i < 4; i++) {
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



