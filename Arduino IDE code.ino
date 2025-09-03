#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

// ✅ WiFi credentials
const char* ssid = "XARVIS 2.4G";
const char* password = "february@2025";

// ✅ Your Django server URL: update IP!
String serverName = "http://192.168.10.192:8000/add/";

// ✅ DHT setup
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("Failed to read from DHT");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" Humidity: ");
    Serial.println(hum);

    WiFiClient client;

    http.begin(client, serverName);  
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // ✅ FORM type!

    String formPayload = "temperature=" + String(temp) + "&humidity=" + String(hum);

    int httpResponseCode = http.POST(formPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(20000); // wait 20 seconds
}
