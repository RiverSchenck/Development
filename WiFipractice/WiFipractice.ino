#include <WiFi.h>

const char* ssid = "wifi";
const char* password = "password";


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected successfully to the WiFi network");
  Serial.println(WiFi.localIP());



}

void loop() {}
