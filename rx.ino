#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR SENDER MAC Address
uint8_t senderAddress[] = {0xD4, 0x8A, 0xFC, 0x9D, 0x5D, 0xB8};

const int in1Pin = 23;
const int in2Pin = 22;
const int in3Pin = 19;
const int in4Pin = 18;

Servo servo1;
Servo servo2;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool buttonState1;
  bool buttonState2;
  bool buttonState3;
  bool buttonState4;
  int potValue;
} struct_message;

// Create a struct_message called receivedData
struct_message receivedData;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  // Copy data from buffer to struct_message variable
  memcpy(&receivedData, data, sizeof(receivedData));

  // Invert button states
  receivedData.buttonState1 = !receivedData.buttonState1;
  receivedData.buttonState2 = !receivedData.buttonState2;
  receivedData.buttonState3 = !receivedData.buttonState3;
  receivedData.buttonState4 = !receivedData.buttonState4;

  // Map potentiometer value from 0-4096 to 0-180
  int mappedPotValue = map(receivedData.potValue, 0, 4096, 0, 180);

  // Process received data
  Serial.print("Button State 1: ");
  Serial.println(receivedData.buttonState1);
  Serial.print("Button State 2: ");
  Serial.println(receivedData.buttonState2);
  Serial.print("Button State 3: ");
  Serial.println(receivedData.buttonState3);
  Serial.print("Button State 4: ");
  Serial.println(receivedData.buttonState4);
  Serial.print("Potentiometer Value: ");
  Serial.println(mappedPotValue);

  servo1.write(mappedPotValue);
  servo2.write(180 - mappedPotValue);

  if (receivedData.buttonState1) { // moveForward
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
  } else if (receivedData.buttonState2) { // turnLeft
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
  } else if (receivedData.buttonState3) { // turnRight
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
  } else if (receivedData.buttonState4) { // backward
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);
  }

}

void setup() {
  Serial.begin(115200);

  servo1.attach(12);
  servo2.attach(14);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function for receiving data
  esp_now_register_recv_cb(OnDataRecv);

  // Add sender as a peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Your code here if you need to do something continuously
}
