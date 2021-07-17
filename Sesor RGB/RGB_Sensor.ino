#define DEBUG

#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() {

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    if (!tcs.begin()) {

        Serial.println("Error al iniciar TCS34725");
    while (1) delay(1000);
    }

    Serial.println("Started");
}

void loop() {
    
    uint16_t r, g, b, c, colorTemp, lux;
  
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);
    
    Serial.print("Temperatura color: "); Serial.print(colorTemp, DEC); Serial.println(" K");
    Serial.print("Lux : "); Serial.println(lux, DEC);
    Serial.print("Rojo: "); Serial.println(r, DEC);
    Serial.print("Verde: "); Serial.println(g, DEC);
    Serial.print("Azul: "); Serial.println(b, DEC);
    Serial.print("Clear: "); Serial.println(c, DEC);
    Serial.println(" ");
    delay(1000);
}