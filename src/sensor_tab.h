#ifndef SENSOR_TAB_H
#define SENSOR_TAB_H

#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11

extern DHT dht;

void setupSensor();
float getTemperatuur();
float getVochtigheid();

#endif