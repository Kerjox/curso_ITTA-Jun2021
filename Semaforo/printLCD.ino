/**
 * @brief 
 * 
 * @param lcd 
 * @param time 
 * @param data 
 * @param side 
 */
void printLcd(DigitLedDisplay lcd, uint32_t &time, uint32_t data, bool side) {

    if (time >= timesLCDTimer.interval + timesLCDTimer.lastExecution) {
        
        lcd.clear();
        if (side) {

            lcd.printDigit(data, 0);
        } else {

            lcd.printDigit(data, 4);
        }

        timesLCDTimer.lastExecution = time;
    }
}

void printLcd(DigitLedDisplay lcd, uint32_t &time, uint8_t cars, uint8_t walkers) {

    if (time >= infoRoadLCDTimer.interval + infoRoadLCDTimer.lastExecution) {
        
        lcd.clear();
        lcd.printDigit(cars, 4);
        lcd.printDigit(walkers, 0);

        infoRoadLCDTimer.lastExecution = time;
    }
}
/*
void printLcd(DigitLedDisplay lcd, uint32_t &time, uint) {

    if (time >= timesLCDTimer.interval + timesLCDTimer.lastExecution) {
        
        lcd.clear();
        lcd.printDigit(data);
        Serial.println(data);

        timesLCDTimer.lastExecution = time;
    }
}*/