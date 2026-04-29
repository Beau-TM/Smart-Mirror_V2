#include <WiFi.h>
#include <WebServer.h>
#include "webserver_tab.h"

const char* ssid = "OnePlus Nord 3 5G";
const char* password = "y893cmpn";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Netwerken scannen...");
  int aantalNetwerken = WiFi.scanNetworks();
  Serial.print(aantalNetwerken);
  Serial.println(" netwerken gevonden");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);

  int pogingen = 0;
  while (WiFi.status() != WL_CONNECTED && pogingen < 20) {
    delay(500);
    Serial.print(".");
    pogingen++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi verbonden!");
    Serial.print("IP-adres: ");
    Serial.println(WiFi.localIP());
    setupWebServer();
  } else {
    Serial.println("\nVerbinding mislukt!");
    Serial.print("WiFi status code: ");
    Serial.println(WiFi.status());
  }
}

void loop() {
  handleWebServer();
}