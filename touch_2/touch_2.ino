//                        attiny85
//                   reset:1 -+---+- 8:power
// (on while touching) pb3:2 -+*  +- 7:pb2 (toggled by touch)
//       (touch input) pb4:3 -+   +- 6:pb1 (fading while touching)
//                  ground:4 -+---+- 5:pb0 (fading always)

byte touchPins[] ={ 2,1 };//, PB5};
byte ledPins[] = { 3,4,2 };

// Calibrate each pin separately.
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
  
  /*for(j = 0; j < sizeof(touchPins); j++) {
    
    Serial.println(calibration[j]);
  }*/  
  
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
    
    /*
    Serial.print("pin");
    Serial.print(i);
    Serial.print(": val");
    Serial.println(n);
    */
    
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


