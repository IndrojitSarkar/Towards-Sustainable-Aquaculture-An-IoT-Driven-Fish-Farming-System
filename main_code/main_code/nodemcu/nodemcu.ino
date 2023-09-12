#include <ESP8266WiFi.h>

String apiKey = "5M5WG5E6863A4GNT";  // Enter your Write API key from ThingSpeak
const char* ssid = "Two Thirty One";   // Replace with your Wi-Fi credentials
const char* password = "201806086";
const char* server = "api.thingspeak.com";

float temperatureC;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  connectWiFi();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the temperature data from Arduino via Serial
    String data = Serial.readStringUntil(';');
    if (data.startsWith("T:")) {
      data.remove(0, 2);
      temperatureC = data.toFloat();
      Serial.print("Received temperature from Arduino: ");
      Serial.print(temperatureC);
      Serial.println("°C");

      // Send the temperature data to ThingSpeak
      sendDataToThingSpeak(temperatureC);

      // ThingSpeak requires a minimum 15 sec delay between updates
      delay(15000);
    }
  }
}

void sendDataToThingSpeak(float temperature) {
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature sent to ThingSpeak: ");
    Serial.println(temperature);
  }
  client.stop();
}
