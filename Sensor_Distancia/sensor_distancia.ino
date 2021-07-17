//#define DEBUG

#include <NewPing.h>
#include <LiquidCrystal.h>

struct Timer {

    uint16_t interval;
    uint32_t last_execution;

};

uint8_t TRIGGER_PIN = 9;
uint8_t ECHO_PIN = 10;
uint8_t MAXDISTANCE = 200;
uint16_t CONT = 0;
uint32_t time;

bool detected = false;
bool counted = false;

NewPing sensor(TRIGGER_PIN, ECHO_PIN, MAXDISTANCE);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Timer lcdPrint;
Timer detection;
Timer lastDetection;

void setup() {

    lcdPrint.interval = 1000;
    detection.interval = 100;
    lastDetection.interval = 50;

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    lcd.begin(16, 2);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distan: ");
    lcd.setCursor(0, 1);
    lcd.print("Detecciones: ");
}

void loop() {

    time = millis();

    uint16_t distancia;

    if (time >= detection.last_execution + detection.interval) {
        
        distancia = sensor.ping_cm();

        if (distancia <= MAXDISTANCE && distancia > 0 && !counted) {
            
            if (detected) {
                
                CONT++;
                counted = true;
                
            } else {

                detected = true;
            }

            lastDetection.last_execution = time;
        } else if (time >= lastDetection.last_execution + lastDetection.interval && distancia == 0) {

            counted = false;
            detected = false;
        }

        Serial.print("Distancia: ");
        Serial.print(distancia);
        Serial.println("cm");

        detection.last_execution = time;
    }

    if (time >= lcdPrint.last_execution + lcdPrint.interval || (detected && !counted)) {

        String lineOne = (String) distancia;
        lineOne.concat(" cm      ");

        String lineTwo = (String) CONT;
        lineTwo.concat("       ");

        lcd.setCursor(8, 0);
        lcd.print(lineOne);
        lcd.setCursor(13, 1);
        lcd.print(lineTwo);

        lcdPrint.last_execution = time;
    }
}