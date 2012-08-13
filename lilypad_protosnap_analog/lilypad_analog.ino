int sensorPin = A6;
int outputPin = 3; // Vibration Motor

void setup() {

    // We want to read information _into_ the sensorPin. 
    pinMode(INPUT, sensorPin);

}

void loop() {
    // Store the value read on the sensors.
    int val = analogRead(sensorPin);
    
    // The sensor pin gives us a value from 0 to 1024 (2^10), but we can 
    // only write a value from 0 to 255 (2^8). This function calls maps
    // one set of values onto another.
    int mapped_val = map(val, 0, 1024, 0, 255);
    
    // Write the mapped value to the output pin, which is connected to something
    // interesting.
    analogWrite(outputPin, mapped_val);
}
