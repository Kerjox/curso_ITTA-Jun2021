#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel waitWalkerStripes[2] = {

    Adafruit_NeoPixel(14, 8, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(14, 9, NEO_GRB + NEO_KHZ800)

};


Adafruit_NeoPixel walkerAssistantStripes[1] = {

    Adafruit_NeoPixel(8, 10, NEO_GRB + NEO_KHZ800)

};