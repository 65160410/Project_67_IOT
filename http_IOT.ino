  #ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid     = "vivo-pin";
const char* password = "112233445566778899";

// REPLACE with your Domain name and URL path or IP address with path
// const char* serverName = "https://staff.informatics.buu.ac.th/~ponlawat.ch/test/dbwrite.php";
//const char* serverName = "https://staff.informatics.buu.ac.th/~ponlawat.ch/Student_work/dbwrite.php";
 const char* serverName = "https://angsila.informatics.buu.ac.th/~65160410/Student_work/dbwrite.php";
//  const char* serverName = "https://angsila.informatics.buu.ac.th/~64160207/TEST/dbwrite.php";
 
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
String apiKeyValue = "tPmAT5Ab3j7F9";

// Sensor details
String sensorName = "DHT11";
String sensorLocation = "Office";


// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

void setup() {

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  //pinMode(potPin,INPUT);
  // (you can also pass in a Wire library object like &Wire2)
 // bool status = bme.begin(0x76);
  //if (!status) {
    //Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
   // while (1);
  //}
}

void loop() {


  //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    // Read DHT11 sensor values
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
  // print out the value you read:
    Serial.println(temperature);
    Serial.println(humidity);
    delay(500);
    // Your Domain name with URL path or IP address with path
    // http.begin(serverName);
    


    // Your Domain name with URL path or IP address with path
    http.begin("https://angsila.informatics.buu.ac.th/~65160410/Student_work/dbwrite.php");
   // http.begin(client,"http://localhost/test_http/dbwrite.php");
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(temperature, 2)
                          + "&value2=" + String(humidity, 2);
    //String httpRequestData = "POST http://localhost/test_http/dbwrite.php?&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
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
  //Send an HTTP POST request every 30 seconds
  delay(3000);  
}
