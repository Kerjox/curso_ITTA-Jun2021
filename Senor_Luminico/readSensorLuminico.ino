#define LIGHT_SENSOR A0

void setup() {
    
    Serial.begin(9600);

    Serial.println("Started");

    pinMode(LIGHT_SENSOR, INPUT);
}

void loop() {
    
    uint16_t data = analogRead(LIGHT_SENSOR);
    Serial.println(data);
    delay(100);
}