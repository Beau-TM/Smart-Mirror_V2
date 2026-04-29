#include <Arduino.h>
#include <WebServer.h>
#include "webserver_tab.h"
#include "sensor_tab.h"

WebServer server(80);

// ============================================================
// CSS - Opmaak van de webpagina
// ============================================================
const char CSS[] = R"(
  * { margin: 0; padding: 0; box-sizing: border-box; }

  /* Achtergrond en lettertype */
  body {
    font-family: 'Segoe UI', sans-serif;
    background: linear-gradient(135deg, #0f0f1a, #1a1a2e);
    color: white;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
  }

  /* Titel bovenaan */
  h1 {
    font-size: 2.5rem;
    letter-spacing: 4px;
    text-transform: uppercase;
    margin-bottom: 2rem;
    color: #a0c4ff;
  }

  /* Container voor de kaarten */
  .kaarten {
    display: flex;
    gap: 2rem;
    flex-wrap: wrap;
    justify-content: center;
    margin-bottom: 2rem;
  }

  /* Individuele kaart (temperatuur, vocht, tijd) */
  .kaart {
    background: rgba(255,255,255,0.05);
    border: 1px solid rgba(255,255,255,0.1);
    border-radius: 20px;
    padding: 2rem 3rem;
    text-align: center;
    backdrop-filter: blur(10px);
    min-width: 180px;
  }

  /* Emoji icoon in elke kaart */
  .kaart .icoon { font-size: 3rem; margin-bottom: 0.5rem; }

  /* Grote waarde zoals 21.5 °C */
  .kaart .waarde {
    font-size: 2.5rem;
    font-weight: bold;
    color: #a0c4ff;
  }

  /* Klein label onder de waarde */
  .kaart .label {
    font-size: 0.9rem;
    color: rgba(255,255,255,0.5);
    margin-top: 0.3rem;
    letter-spacing: 2px;
    text-transform: uppercase;
  }

  /* Rode waarschuwingsbalk als vocht te hoog is */
  .waarschuwing {
    background: rgba(255, 80, 80, 0.2);
    border: 1px solid rgba(255, 80, 80, 0.5);
    border-radius: 12px;
    padding: 1rem 2rem;
    color: #ff8080;
    font-size: 1rem;
    margin-bottom: 1.5rem;
  }

  /* Klein tekstje onderaan over de refresh */
  .refresh {
    font-size: 0.8rem;
    color: rgba(255,255,255,0.3);
    margin-top: 1rem;
  }
)";

// ============================================================
// JAVASCRIPT - Automatisch refreshen en tijd bijhouden
// ============================================================
const char JS[] = R"(
  // Herlaad de pagina elke 5 seconden voor nieuwe sensorwaarden
  setTimeout(() => location.reload(), 5000);

  // Update de tijd elke seconde zonder te refreshen
  function updateTijd() {
    const nu = new Date();
    const tijdStr = nu.toLocaleTimeString('nl-BE');
    const el = document.getElementById('tijd');
    if (el) el.innerText = tijdStr;
  }
  setInterval(updateTijd, 1000);
  window.onload = updateTijd;
)";

// ============================================================
// HTML - Opbouw van de pagina
// ============================================================
void handleRoot() {
  // Sensorwaarden ophalen
  float t = getTemperatuur();
  float h = getVochtigheid();

  // Waarschuwing tonen als luchtvochtigheid te hoog is (>65%)
  String waarschuwing = "";
  if (h >= 65.0) {
    waarschuwing = "<div class='waarschuwing'>⚠️ Luchtvochtigheid te hoog! Open een raam.</div>";
  }

  // Volledige HTML pagina opbouwen
  String html = "<!DOCTYPE html><html lang='nl'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Smart Mirror</title>";

  // CSS injecteren
  html += "<style>" + String(CSS) + "</style>";

  // JavaScript injecteren
  html += "<script>" + String(JS) + "</script>";

  html += "</head><body>";
  html += "<h1>🪞 Smart Mirror</h1>";

  // Waarschuwing tonen indien van toepassing
  html += waarschuwing;

  // Kaarten met sensordata
  html += "<div class='kaarten'>";

  // Temperatuur kaart
  html += "<div class='kaart'>";
  html += "<div class='icoon'>🌡️</div>";
  html += "<div class='waarde'>" + String(t, 1) + " °C</div>";
  html += "<div class='label'>Temperatuur</div>";
  html += "</div>";

  // Luchtvochtigheid kaart
  html += "<div class='kaart'>";
  html += "<div class='icoon'>💧</div>";
  html += "<div class='waarde'>" + String(h, 0) + " %</div>";
  html += "<div class='label'>Luchtvochtigheid</div>";
  html += "</div>";

  // Tijd kaart (wordt bijgehouden door JavaScript)
  html += "<div class='kaart'>";
  html += "<div class='icoon'>🕐</div>";
  html += "<div class='waarde' id='tijd'>--:--:--</div>";
  html += "<div class='label'>Tijd</div>";
  html += "</div>";

  html += "</div>"; // einde kaarten

  html += "<div class='refresh'>Pagina refresht automatisch elke 5 seconden</div>";
  html += "</body></html>";

  // Pagina versturen naar de browser
  server.send(200, "text/html", html);
}

// ============================================================
// WEBSERVER SETUP - Routes registreren en server starten
// ============================================================
void setupWebServer() {
  // Hoofdpagina route
  server.on("/", handleRoot);

  // 404 pagina als route niet gevonden wordt
  server.onNotFound([]() {
    server.send(404, "text/plain", "Pagina niet gevonden");
  });

  server.begin();
  Serial.println("Webserver gestart!");
}

// ============================================================
// WEBSERVER LOOP - Moet elke loop() aangeroepen worden
// ============================================================
void handleWebServer() {
  server.handleClient();
}