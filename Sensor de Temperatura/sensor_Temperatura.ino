#define DEBUG

#include <DHT.h>;
#include <LiquidCrystal.h>

#define DHTPIN 11
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

float hum;
float temp;

void setup() {
    
    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    dht.begin();
    lcd.begin(16, 2);
}

void loop() {
    
    temp = dht.readTemperature();
    hum = dht.readHumidity();

    String msg = "------------------------------\n";
    msg.concat("Temperatura: ");
    msg.concat(temp);
    msg.concat(" ºC\n");
    msg.concat("Humedad: ");
    msg.concat(hum);
    msg.concat(" %\n");
    msg.concat("------------------------------");
    Serial.println(msg);

    String lineOne = "Temp: ";
    lineOne.concat(temp);
    lineOne.concat(" C");

    String lineTwo = "Hum: ";
    lineTwo.concat(hum);
    lineTwo.concat(" %");

    lcd.setCursor(0, 0);
    lcd.print(lineOne);
    lcd.setCursor(0, 1);
    lcd.print(lineTwo);

    delay(2000);
}