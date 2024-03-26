#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

  #ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Arduino.h>
#include <SHTC3.h>
#include <TinyGPS++.h>


#define GPS_BAUDRATE 9600
const int ldrPin = 35; 

SHTC3 shtc3(Wire);
TinyGPSPlus gps;  // the TinyGPS++ object
const char* ssid     = "Room603_2.4G";
const char* password = "room9999";
const char* serverName = "https://angsila.informatics.buu.ac.th/~65160410/TectIOT/dbwrite.php";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "PROJECT";


double latitude = 0.0; // Initialize latitude outside any conditional block
double longitude = 0.0; // Initialize longitude outside any conditional block

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

  Serial.begin(9600);
  Serial2.begin(GPS_BAUDRATE);
  analogReadResolution(12);
  Wire.begin(); // Initialize I2C communication
  shtc3.begin(true); // Begin communication with the SHTC3 sensor

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
 
    if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    while (Serial2.available()) {
    if (gps.encode(Serial2.read())) {
      // Check if GPS fix is available
      if (gps.location.isValid()) {
        // Get latitude and longitude as floating-point values
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        }
      }
    }

    int sensorValue = analogRead(ldrPin); // Read analog value from LDR
    float lightIntensity = map(sensorValue, 0, 4095, 0, 100);
    shtc3.sample();
    float temperature = shtc3.readTempC();
    float humidity = shtc3.readHumidity();

    String Location = String(latitude, 7) + ", " + String(longitude, 7); 

    Serial.print("Location: ");
    Serial.print(Location);
    Serial.println(" ");

    Serial.print("SHTC3 Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("SHTC3 Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("LDR: ");
    Serial.print(lightIntensity);
    Serial.println(" ");
 
    http.begin("https://angsila.informatics.buu.ac.th/~65160410/TectIOT/dbwrite.php");

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=" + apiKeyValue +
                         "&sensor=" + sensorName +
                         "&location=" + Location +
                         "&lightIntensity=" + (lightIntensity, 2) +
                         "&temperature=" + String(temperature, 2) +
                         "&humidity=" + String(humidity, 2);
                               
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(1000 * (60 * 5)); 
}
