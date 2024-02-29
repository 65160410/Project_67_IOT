#include <TinyGPS++.h>

#define GPS_BAUDRATE 9600  // The default baudrate of NEO-6M is 9600

TinyGPSPlus gps;  // the TinyGPS++ object

void setup() {
  Serial.begin(9600);
  Serial2.begin(GPS_BAUDRATE);

  Serial.println(F("ESP32 - GPS module"));
}

void loop() {
  // Read data from the GPS module
  while (Serial2.available()) {
    if (gps.encode(Serial2.read())) {
      // Check if GPS fix is available
      if (gps.location.isValid()) {
        // Get latitude and longitude as floating-point values
        double latitude = gps.location.lat();
        double longitude = gps.location.lng();

        // Print latitude and longitude
        Serial.print(F("Latitude: "));
        Serial.println(latitude, 7);  // Print with 7 decimal places
        Serial.print(F("Longitude: "));
        Serial.println(longitude, 7);  // Print with 7 decimal places
      }
    }
  }
  if (!gps.location.isValid()) {
    Serial.println("Waiting for valid GPS data...");
  }
  delay(5000);
}
