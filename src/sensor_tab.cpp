#include <Arduino.h>
#include <DHT.h>
#include "sensor_tab.h"

DHT dht(DHTPIN, DHTTYPE);

void setupSensor() {
  dht.begin();
}

float getTemperatuur() {
  return dht.readTemperature();
}

float getVochtigheid() {
  return dht.readHumidity();
}