#include <ESP32Servo.h>

#define PIN_TRIG 32
#define PIN_ECHO 35
#define SERVO_PIN_1 14
#define SERVO_PIN_2 25

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);

  // Set initial positions for servo motors
  servo1.write(90); // Change the angle (0-180) to adjust initial position
  servo2.write(90); // Change the angle (0-180) to adjust initial position

  delay(1000); // Delay for servo motors to reach initial positions
}

void loop() {
  long duration, distance;

  // Trigger the ultrasonic sensor
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure the echo duration
  duration = pulseIn(PIN_ECHO, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Move servo motors based on distance
  if (distance <= 30) {
    servo1.write(0);  // Move servo 1 to 0 degrees
    servo2.write(180); // Move servo 2 to 180 degrees
  } else {
    servo1.write(90); // Move servo 1 to 90 degrees
    servo2.write(90); // Move servo 2 to 90 degrees
  }

  delay(1000); // Delay for the servos to reach the desired position
}
