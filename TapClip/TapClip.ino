#include <CapSense.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

#define MAX 500
#define PIN1 8
#define PIN2 9

CapSense   cs_4_2 = CapSense(A3,A2);        
CapSense   cs_4_6 = CapSense(A1,A0);       
CapSense   cs_4_8 = CapSense(A5,A4);      

void setup()                    
{
  // cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   Serial.begin(9600);
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_4_2.capSense(30);
    long total2 =  cs_4_6.capSense(30);
    long total3 =  cs_4_8.capSense(30);

    //Serial.print(millis() - start);        // check on performance in milliseconds
    //Serial.print(",");                    // tab character for debug windown spacing

    Serial.print(total2);                  // print sensor output 1
    Serial.print(",");
    Serial.print(total1);                  // print sensor output 2
    Serial.print(",");
    Serial.println(total3);                // print sensor output 3

    if((total1 > MAX)|(total2 > MAX)|(total3 > MAX)) {
        digitalWrite(PIN1, HIGH);
    } else {
        digitalWrite(PIN1, LOW);
    }
    delay(10);
}
