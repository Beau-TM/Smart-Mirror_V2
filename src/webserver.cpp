#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h> // De compiler MOET dit nu zien via de ESP32-core
#include "webserver.h"

// We definiëren de server hier opnieuw heel expliciet
WebServer server(80);

void setupWebServer() {
  server.on("/", []() {
    server.send(200, "text/html", "<h1>Smart Mirror V2</h1><p>Verbinding werkt!</p>");
  });
  server.begin();
  Serial.println("Webserver gestart!");
}

void handleWebServer() {
  server.handleClient();
}