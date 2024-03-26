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

const char* ssid     = "vivo-pin";
const char* password = "12345678";
const char* serverName = "https://angsila.informatics.buu.ac.th/~65160410/TectIOT/dbwrite.php";

String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "PROJECT";

double latitude = 0.0; // Initialize latitude outside any conditional block
double longitude = 0.0; // Initialize longitude outside any conditional block

void setup() {

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
                         "&lightIntensity=" + String(lightIntensity, 2) +
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
  delay(300000);  //5 นาที
}
