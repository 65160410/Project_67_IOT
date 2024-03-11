#include <ESP32Servo.h>

#define PIN_TRIG 32
#define PIN_ECHO 35
#define SERVO_PIN_1 14
#define SERVO_PIN_2 25

#define sensorLeftPin 32
#define sensorRightPin 35
#define in1Pin 23
#define in2Pin 22
#define in3Pin 19
#define in4Pin 18

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);

  setInitialPositions();
}

void loop() {
  long duration, distance;

  triggerSensor();

  duration = measureEchoDuration();
  distance = calculateDistance(duration);

  printDistance(distance);

  if (distance <= 30) {
    moveServosUp(); // Move servos up
  } else {
    moveServosDown(); // Move servos down
  }

  // Read sensor values
  int sensorLeft = digitalRead(sensorLeftPin);
  int sensorRight = digitalRead(sensorRightPin);

  if (sensorLeft == LOW && sensorRight == LOW) {
    // If both sensors detect nothing, stop the motors
    motorStop();
  } else if (sensorLeft == HIGH && sensorRight == HIGH) {
    // If both sensors detect an obstacle, move forward
    moveForward();
  } else if (sensorLeft == HIGH) {
    // If only left sensor detects an obstacle, turn left
    turnLeft();
  } else if (sensorRight == HIGH) {
    // If only right sensor detects an obstacle, turn right
    turnRight();
  }

  delay(100); // Adjust delay as needed
}

void setInitialPositions() {
  servo1.write(90);
  servo2.write(90);
  delay(1000);
}

void triggerSensor() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
}

long measureEchoDuration() {
  return pulseIn(PIN_ECHO, HIGH);
}

long calculateDistance(long duration) {
  return duration * 0.034 / 2;
}

void printDistance(long distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void moveServosUp() {
  servo1.write(0);  // Move servo 1 up to 0 degrees
  servo2.write(180); // Move servo 2 up to 180 degrees
}

void moveServosDown() {
  servo1.write(90); // Move servo 1 down to 90 degrees
  servo2.write(90); // Move servo 2 down to 90 degrees
}

void moveForward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
}

void motorStop() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
}

void turnLeft() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
}

void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
}
