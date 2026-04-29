#include <Arduino.h>
#include "motion_tab.h"
#include "display_tab.h"

#define PIR_PIN 13

bool bewegingGezien = false;

void setupMotion() {
  pinMode(PIR_PIN, INPUT);
}

void handleMotion() {
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