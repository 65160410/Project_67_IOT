#include <ESP32Servo.h>

// Create a servo object
Servo myservo;

void setup() {
  // Attach the servo to pins 
  myservo.attach(12);
  myservo.attach(14);

  // You can also set the minimum and maximum pulse width if needed
  // myservo.attach(34, 500, 2500);

  // Optional: You can set the initial position of the servo (in degrees)
  // myservo.write(90);

  Serial.begin(115200);
  Serial.println("Servo control example");
}

void loop() {
  // Move the servo to different positions
  myservo.write(0);  // Move to 0 degrees
  delay(1000);
  myservo.write(0); // Move to 90 degrees
  myservo.write(150); // Move to 180 degrees
  delay(1000);
}
