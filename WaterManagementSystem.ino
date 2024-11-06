
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server = "http://example.com/update";

const int tdsPin = A0;
const int waterLevelPin = D2;
float calibrationFactor = 1.0;
unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  pinMode(waterLevelPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int tdsValue = analogRead(tdsPin);
    float tds = (tdsValue * calibrationFactor) / 1024.0 * 5.0 * 1000;
    int waterLevel = digitalRead(waterLevelPin);

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, server);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "tds=" + String(tds) + "&water_level=" + String(waterLevel);
      http.POST(httpRequestData);
      http.end();
    }
  }
}
