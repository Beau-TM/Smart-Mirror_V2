#ifndef DISPLAY_TAB_H
#define DISPLAY_TAB_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void setupDisplay();
void toonGroet();
void lusInformatie();

#endif