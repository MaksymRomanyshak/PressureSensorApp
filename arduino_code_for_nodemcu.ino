#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>


const char* ssid = "SSID";       // Replace with your Wi-Fi name
const char* password = "PASSWORD"; // Replace with your Wi-Fi password
const char* serverAddress = "https://pressure-sensor.up.railway.app/data"; // your local ip "http://your-local-ip:3000/data" or deployed URL

const int sensorPin = A0; // Input pin for the sensor
const float VCC = 3.3;    // NodeMCU ADC reference voltage
const float sensorVmax = 5;  // Voltage at max pressure (101.5 psi)
const float sensorVmin = 0.90;  //  Voltage at 0 psi (no pressure)
const float Pmax = 101.5; // Maximum pressure in psi

WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = analogRead(sensorPin); // Read ADC value
  float measuredVoltage = ((sensorValue) / 1023.0) * VCC; // Convert ADC to voltage
  float pressurePsi = ((measuredVoltage - sensorVmin) / (sensorVmax - sensorVmin)) * Pmax;

  Serial.print("Voltage: ");
  Serial.print(measuredVoltage, 2);
  Serial.print("V | Pressure: ");
  Serial.print(pressurePsi, 2);
  Serial.println(" psi");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();
    http.begin(client, serverAddress);
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["pressure"] = pressurePsi;
    jsonDoc["voltage"] = measuredVoltage;
    jsonDoc["status"] = "OK";
    
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    int httpResponseCode = http.POST(requestBody);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);
    } else {
      Serial.println("Error in HTTP request");
    }
    
    http.end();
  }

  delay(1000);
}
