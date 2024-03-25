#include <esp_now.h>
#include <WiFi.h>

const int buttonPin1 = 32;
const int buttonPin2 = 26;
const int buttonPin3 = 25;
const int buttonPin4 = 33;
const int potPin = 35;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xEC, 0x64, 0xC9, 0x86, 0x39, 0x20};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool buttonState1;
  bool buttonState2;
  bool buttonState3;
  bool buttonState4;
  int potValue;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Set pin modes for buttons
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
}

void loop() {
  // Read button states
  myData.buttonState1 = digitalRead(buttonPin1);
  myData.buttonState2 = digitalRead(buttonPin2);
  myData.buttonState3 = digitalRead(buttonPin3);
  myData.buttonState4 = digitalRead(buttonPin4);

  // Read potentiometer value
  myData.potValue = analogRead(potPin);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(100);
}
