#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "webserver_tab.h"
#include "sensor_tab.h"
#include "display_tab.h"
#include "motion_tab.h"

const char* ssid     = "OnePlus Nord 3 5G";
const char* password = "y893cmpn";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);
  
  setupSensor();
  setupDisplay();
  setupMotion();

  lcd.setCursor(0, 0);
  lcd.print("Verbinden...");

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
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    setupWebServer();
  } else {
    Serial.println("\nWiFi mislukt, we gaan door zonder tijd.");
  }

  lcd.clear();
  lcd.noBacklight();
}

void loop() {
  handleMotion();
  handleWebServer();
}