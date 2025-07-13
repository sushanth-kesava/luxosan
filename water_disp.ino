const int irSensorPin = 2;   // IR Sensor connected to digital pin 2
const int relayPin = 7;      // Relay control pin

void setup() {
  pinMode(irSensorPin, INPUT);   // IR sensor as input
  pinMode(relayPin, OUTPUT);     // Relay as output
  digitalWrite(relayPin, LOW);   // Pump OFF initially
  Serial.begin(9600);
  Serial.println("IR Sensor Pump Control Ready");
}

void loop() {
  int sensorState = digitalRead(irSensorPin);

  Serial.print("IR Sensor State: ");
  Serial.println(sensorState);

  if (sensorState == LOW) {  // Obstacle detected (LOW on most IR modules)
    Serial.println("Hand Detected - Pump ON");
    digitalWrite(relayPin, LOW); // Turn ON pump
  } else {  // No obstacle
    Serial.println("No Hand - Pump OFF");
    digitalWrite(relayPin, HIGH);  // Turn OFF pump
  }

  delay(100);  // Small delay to reduce bouncing
}