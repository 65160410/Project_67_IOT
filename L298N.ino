const int sensorLeftPin = 32;
const int sensorRightPin = 35;
const int in1Pin = 23;
const int in2Pin = 22;
const int in3Pin = 19;
const int in4Pin = 18;
const int ENA = 26;
const int ENB = 27;

void setup() {
  // กำหนดพินเป็นอินพุตหรือเอาต์พุต
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  // อ่านค่าจากเซนเซอร์
  int sensorLeft = digitalRead(sensorLeftPin);
  int sensorRight = digitalRead(sensorRightPin);

  if (sensorLeft == LOW && sensorRight == LOW) {
  // กรณีทั้งคู่ตรวจจับได้พร้อมกัน ให้เคลื่อนที่ไปข้างหน้า
  motorStop();
  }else if (sensorLeft == HIGH && sensorRight == HIGH) {
  // กรณีทั้งคู่ตรวจจับได้พร้อมกัน ให้เคลื่อนที่ไปข้างหน้า
  moveForward();
  } else if (sensorLeft == HIGH) {
  // กรณีเซนเซอร์ซ้ายตรวจจับได้ ให้เลี้ยวซ้าย
  turnLeft();
  } else if (sensorRight == HIGH) {
  // กรณีเซนเซอร์ขวาตรวจจับได้ ให้เลี้ยวขวา
  turnRight();
  } 
}

void motorStop() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
}

void moveForward() {
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
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
}

void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
}
