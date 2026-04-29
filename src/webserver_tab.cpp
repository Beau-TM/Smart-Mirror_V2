#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Smart Mirror</h1><p>Webserver werkt!</p>");
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Webserver gestart!");
}

void handleWebServer() {
  server.handleClient();
}