#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include "time.h"

// --- CONFIGURATIE ---
const char* ssid     = "OnePlus Nord 3 5G";
const char* password = "y893cmpn";

#define DHTPIN 14
#define DHTTYPE DHT11
const int PIR_PIN = 13;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      
const int   daylightOffset_sec = 3600; 

bool bewegingGezien = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  dht.begin();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Verbinden...");

  // WiFi met een timeout (zodat de code niet vastloopt als wifi faalt)
  WiFi.begin(ssid, password);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED && counter < 20) {
    delay(500);
    Serial.print(".");
    counter++;
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi verbonden!");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  } else {
    Serial.println("\nWiFi mislukt, we gaan door zonder tijd.");
  }

  lcd.clear();
  lcd.noBacklight();
}

void toonGroet() {
  struct tm timeinfo;
  lcd.setCursor(0, 0);
  if(!getLocalTime(&timeinfo)){
    lcd.print("Hallo Lenn!"); // Backup tekst als internet faalt
  } else {
    int uur = timeinfo.tm_hour;
    if(uur >= 0 && uur < 12) lcd.print("Goeiemorgen!");
    else if(uur >= 12 && uur < 17) lcd.print("Goeiemiddag!");
    else lcd.print("Goeieavond!");
  }
  delay(2000);
  lcd.clear();
}

void lusInformatie() {
  while(digitalRead(PIR_PIN) == HIGH) {
    // --- STAP 1: TIJD TONEN ---
    struct tm timeinfo;
    if(getLocalTime(&timeinfo)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Het is nu:");
      lcd.setCursor(0, 1);
      char tijdStr[9];
      sprintf(tijdStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      lcd.print(tijdStr);
      delay(3000);
    }
    if(digitalRead(PIR_PIN) == LOW) break;

    // --- STAP 2: TEMP & VOCHT TONEN ---
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check of de sensor wel werkt (geen 'nan')
    if (!isnan(h) && !isnan(t)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: "); lcd.print(t, 1); lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Vocht: "); lcd.print(h, 0); lcd.print("%");
      delay(3000);

      // --- STAP 3: ALLEEN ADVIES ALS VOCHT > 65% ---
      if (h >= 65.0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Advies:ventileer");
        lcd.setCursor(0, 1);
        lcd.print("open een raam");
        delay(4000);
      }
    } else {
      Serial.println("DHT11 fout: Check bedrading!");
    }
    
    if(digitalRead(PIR_PIN) == LOW) break;
  }
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    if (!bewegingGezien) {
      lcd.backlight();
      toonGroet(); 
      bewegingGezien = true;
    }
    lusInformatie(); 
  } else {
    if (bewegingGezien) {
      lcd.clear();
      lcd.noBacklight();
      bewegingGezien = false;
    }
  }
  delay(100);
}