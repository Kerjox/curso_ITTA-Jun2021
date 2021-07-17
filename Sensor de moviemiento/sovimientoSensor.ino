#define DEBUG

uint8_t SENSOR_PIN = 8;
uint8_t LED_PIN = 9;

void setup() {

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    pinMode(SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Started");
}

void loop() {
    
    digitalWrite(LED_PIN, digitalRead(SENSOR_PIN));
    Serial.println(digitalRead(SENSOR_PIN));
}