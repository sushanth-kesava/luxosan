// --------------------- Pin Definitions ---------------------

// Motor Control Pins
#define MOTOR1_PIN1    4
#define MOTOR1_PIN2    5
#define MOTOR2_PIN1    6
#define MOTOR2_PIN2    7

// Motor Enable Pins (PWM)
#define ENABLE_PIN1    3   // Left Motor
#define ENABLE_PIN2    11  // Right Motor

// Ultrasonic Sensor Pins
#define TRIG_PIN       2
#define ECHO_PIN      8

// --------------------- Constants ---------------------

#define STOP_THRESHOLD   80   // Stop if object is within 80 cm
#define STILL_THRESHOLD  5    // Max change in cm to consider object stationary
#define CHECK_DELAY      1000 // Time to wait before second distance check (ms)

#define MOTOR_SPEED      200  // PWM speed (0–255)

void setup() {
  // Set motor control pins as OUTPUT
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  pinMode(ENABLE_PIN1, OUTPUT);
  pinMode(ENABLE_PIN2, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);

  // Set initial motor speed
  setSpeed(MOTOR_SPEED);
}

void loop() {
  long initialDistance = getDistance();

  Serial.print("Initial Distance: ");
  Serial.print(initialDistance);
  Serial.println(" cm");

  if (initialDistance > 0 && initialDistance < STOP_THRESHOLD) {
    stopMotors();
    Serial.println("Object detected. Pausing to check if it's moving...");
    delay(CHECK_DELAY);

    long newDistance = getDistance();
    long change = abs(newDistance - initialDistance);

    Serial.print("New Distance: ");
    Serial.print(newDistance);
    Serial.print(" cm | Change: ");
    Serial.println(change);

    if (change <= STILL_THRESHOLD) {
      Serial.println("Object is not moving. Reversing and turning...");
      reverseAndTurn();
    } else {
      Serial.println("Object is moving. Staying stopped...");
      // Stay stopped
    }
  } else {
    moveForward();
  }

  delay(200);
}

// -------- Movement Functions --------

void setSpeed(int speed) {
  analogWrite(ENABLE_PIN1, speed);
  analogWrite(ENABLE_PIN2, speed);
}

void moveForward() {
  setSpeed(MOTOR_SPEED);
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
}

void reverseAndTurn() {
  // Reverse
  setSpeed(MOTOR_SPEED);
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
  delay(700);

  // Turn Right
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
  delay(600);

  stopMotors();
}

void stopMotors() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
  setSpeed(0);  // Optionally set PWM speed to 0
}

// -------- Ultrasonic Sensor Function --------

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;

  long distance = duration * 0.034 / 2;
  return distance;
}