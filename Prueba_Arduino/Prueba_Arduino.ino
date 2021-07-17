//#define DEBUG

uint8_t LED0 = 7;
uint8_t LED1 = 8;

uint32_t last_Time_LED0 = 0;
uint16_t interval_LED0 = 500;

uint32_t last_Time_LED1 = 0;
uint16_t interval_LED1 = 1000;

uint32_t current_Time;

void setup() {

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
}

void loop() {

    current_Time = millis();

    if (current_Time >= last_Time_LED0 + interval_LED0) {

        digitalWrite(LED0, !digitalRead(LED0));
        last_Time_LED0 = current_Time;
        Serial.println("Led0");
    }

    if (current_Time >= last_Time_LED1 + interval_LED1) {

        digitalWrite(LED1, !digitalRead(LED1));
        last_Time_LED1 = current_Time;
        Serial.println("Led1");
    }
}