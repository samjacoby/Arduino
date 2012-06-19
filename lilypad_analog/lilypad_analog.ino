int sensorPin = A6;

int outputPin = 3; // Vibration Motor

void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600); 

    // We want to read information _into_ the sensorPin. 
    pinMode(INPUT, sensorPin);

}

void loop() {
    int val = analogRead(sensorPin);
    int mapped_val = map(val, 0, 1024, 0, 255);
    Serial.println(mapped_val);
    analogWrite(outputPin, mapped_val);
}
