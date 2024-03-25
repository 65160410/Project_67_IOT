#include <ESP32Servo.h>

// ประกาศวัตถุ Servo
Servo servo1;
Servo servo2;

void setup() {
  // Attach โมเตอร์เซอร์โว กับขา
  servo1.attach(12);
  servo2.attach(14);

  // เซ็ตให้โมเตอร์เซอร์โวหมุนไปที่ 0 องศา
  servo1.write(180);
  servo2.write(0);
  delay(1000);
}

void loop() {
  // ใส่โค้ดอื่นๆ ที่ต้องการในลูปหลัก ถ้ามี
  servo1.write(0);
  servo2.write(180);
  delay(1000);
}
