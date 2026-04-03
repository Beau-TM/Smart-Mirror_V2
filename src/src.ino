#include <WiFi.h>
#include "webserver.h"

// Vul hier je eigen WiFi in
const char* ssid = "JOUW_WIFI_NAAM";
const char* password = "JOUW_WACHTWOORD";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password);
  Serial.print("Verbinden met WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi verbonden!");
  Serial.print("IP-adres: ");
  Serial.println(WiFi.localIP()); 

  setupWebServer();
}

void loop() {
  handleWebServer();
}