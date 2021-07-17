//#define DEBUG

#define TxD 10
#define RxD 11

#include <SoftwareSerial.h>

struct LED {

    bool state;
    uint8_t pin;
    uint8_t brightness;
};

struct Timer {

    uint16_t interval;
    uint32_t last_execution;

};

uint8_t LED0 = 7;
uint8_t LIGHT_SENSOR = A0;

SoftwareSerial bt(TxD, RxD);
LED led1;
Timer lightSensorTimer;

void setup() {

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    bt.begin(9600);

    initLED(led1, false, 6, 0);

    initTimer(lightSensorTimer, 2000);

    pinMode(LED0, OUTPUT);
    pinMode(led1.pin, OUTPUT);
    pinMode(LIGHT_SENSOR, INPUT);
}

void loop() {

    if (bt.available()) {

        String msg = bt.readString();
        Serial.println(msg);
        decodeMessage(msg);
    }

    if (millis() >= lightSensorTimer.last_execution + lightSensorTimer.interval) {
        
        String msg = (String) analogRead(LIGHT_SENSOR);

        bt.write(msg.c_str());
        lightSensorTimer.last_execution = millis();
    }
    

}

void initLED(LED &led, bool state, uint8_t pin, uint8_t brightness) {

    led.state = state;
    led.pin = pin;
    led.brightness = brightness;

}

void initTimer (Timer &timer, uint16_t interval) {

    timer.interval = interval;
}

void decodeMessage(String &msg) {

    if (msg == "led0") {

            digitalWrite(LED0, !digitalRead(LED0));
        } else if (msg == "led1") {

            if (led1.state) {
                
                led1.state = false;
                digitalWrite(led1.pin, led1.state);
            } else {
                
                led1.state = true;
                Serial.println(led1.brightness);
                analogWrite(led1.pin, led1.brightness);
            }
        } else if (msg.substring(0, 16) == "brightness_led1=") {

            led1.brightness = msg.substring(msg.lastIndexOf("=") + 1).toInt();

            Serial.println(led1.brightness);
            analogWrite(led1.pin, led1.brightness);

            if (!led1.state) {
                
                led1.state = true;
            }
        }
}