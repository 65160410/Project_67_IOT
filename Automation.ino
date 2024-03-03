const int sensorLeftPin = 32;   // Left color sensor
const int sensorRightPin = 35;  // Right color sensor
const int relayPin1 = 14;       // Pin to control the left relay
const int relayPin2 = 27;       // Pin to control the right relay

void setup() {
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
}

void stopMovement() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
}

void moveForward() {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}

void turnLeft() {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, HIGH);
}

void turnRight() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, LOW);
}

void loop() {
  int leftSensorValue = digitalRead(sensorLeftPin);
  int rightSensorValue = digitalRead(sensorRightPin);

  if (leftSensorValue == HIGH && rightSensorValue == HIGH) {
    stopMovement();
  } else if (leftSensorValue == LOW && rightSensorValue == LOW) {
    moveForward();
  } else {
    while (leftSensorValue == LOW || rightSensorValue == LOW) {
      if (leftSensorValue == LOW) {
        turnLeft();
      } else if (rightSensorValue == LOW) {
        turnRight();
      }

      // Read sensor values again after turning
      leftSensorValue = digitalRead(sensorLeftPin);
      rightSensorValue = digitalRead(sensorRightPin);

      // Break the loop if both sensors read HIGH
      if (leftSensorValue == HIGH && rightSensorValue == HIGH) {
        break;
      }
    }
  }
}
