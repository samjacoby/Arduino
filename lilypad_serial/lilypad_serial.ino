
int sensorPin = A0;

void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600); 

    // We want to read information _into_ out sensorPin. 
    pinMode(INPUT, sensorPin);
}

void loop() {
    int val = analogRead(A0);
    Serial.println(val);
    // delay 100ms, to avoid jamming up the serial line and get useful values
    delay(100);
}
