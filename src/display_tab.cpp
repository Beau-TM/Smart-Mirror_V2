#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "display_tab.h"
#include "sensor_tab.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupDisplay() {
  lcd.init();
  lcd.backlight();
}

void toonGroet() {
  struct tm timeinfo;
  lcd.setCursor(0, 0);
  if (!getLocalTime(&timeinfo)) {
    lcd.print("Hallo!");
  } else {
    int uur = timeinfo.tm_hour;
    if (uur >= 0 && uur < 12)       lcd.print("Goeiemorgen!");
    else if (uur >= 12 && uur < 17) lcd.print("Goeiemiddag!");
    else                             lcd.print("Goeieavond!");
  }
  delay(2000);
  lcd.clear();
}

void lusInformatie() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Het is nu:");
    lcd.setCursor(4, 1);
    char tijdStr[9];
    sprintf(tijdStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    lcd.print(tijdStr);
    delay(3000);
  }

  float h = getVochtigheid();
  float t = getTemperatuur();

  if (!isnan(h) && !isnan(t)) {
    lcd.clear();
    char tempStr[16];
    sprintf(tempStr, "Temp: %.1fC", t);
    int startTemp = (16 - strlen(tempStr)) / 2;
    lcd.setCursor(startTemp, 0);
    lcd.print(tempStr);

    char vochtStr[16];
    sprintf(vochtStr, "Vocht: %.0f%%", h);
    int startVocht = (16 - strlen(vochtStr)) / 2;
    lcd.setCursor(startVocht, 1);
    lcd.print(vochtStr);
    delay(3000);

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
}