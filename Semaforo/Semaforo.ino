/**
 * @file Semaforo.ino
 * @author Kerjox (https://github.com/Kerjox)
 * @brief 
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>
#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "ledStripes.h"
#include "DigitLedDisplay.h"

struct Semaforo {

    uint8_t CAR_RED_PIN;
    uint8_t CAR_YELLOW_PIN;
    uint8_t CAR_GREEN_PIN;
    uint8_t WALKER_RED_PIN;
    uint8_t WALKER_GREEN_PIN;
    uint8_t animationState;
    bool direction;
    bool started;
};

struct Timer {

    uint32_t interval;
    uint32_t lastExecution;
    bool started;
};

struct BlinkAnimation : Timer {

    uint8_t intervalBlinks;
    uint32_t lastExecutionBlinks;
    uint8_t cont;
    bool blink;
    bool started;
};

BlinkAnimation animations[2] = {};

/**
 * @brief 
 * 0 -> Cars OPEN;
 * 1 -> Cars CLOSE
 */
int8_t state = -1;

uint8_t cars = 1;
uint8_t walkers = 0;

Timer walkersOpenTimeout;
Timer timeWalkersToCross;
Timer animationSemaforo;
Timer timesLCDTimer;
Timer infoRoadLCDTimer;
Timer addWalkersTimer;
Timer stopCars;

Semaforo semaforo;

DigitLedDisplay timesLCD = DigitLedDisplay(11, 12, 13);
DigitLedDisplay infoRoad = DigitLedDisplay(A0, A1, A2);

uint32_t prueba = 0;

void setup() {

    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    initAnimations();
    initLedStripes();
    initSemaforo();
    initLCD();
    initButtons();
}

void loop() {

    uint32_t time = millis();

    switch (state)
    {
    case OPENCARS:

        blinkLeds(waitWalkerStripes, 2, animations[0], 0xFF, 0x00, 0x00, time);
        algoritmo(time);
        addWalkers(time);
        checkStopCars(time);
        break;
    case OPENWALKERS:
        
        blinkLeds(walkerAssistantStripes, 1, animations[1], 0x00, 0xFF, 0x00, time);
        checkTimeoutOpenWalkers(time);
        break;

    case CHANGESEMAFORO:
        
        animateSemaforo(time);
        break;
    
    default:

        state = OPENCARS;
        animateSemaforo(time);
        break;
    }

    printLcd(infoRoad, time, cars, walkers);
}

void blinkLeds(Adafruit_NeoPixel stripes[], uint8_t arrayLength, BlinkAnimation &animation, uint8_t r, uint8_t g, uint8_t b, uint32_t &time) {

    if (!animation.started) {

        for (uint8_t i = 0; i < arrayLength; i++) {
            stripes[i].fill(stripes[i].Color(r, g, b));
            stripes[i].show();
        }
        
        animation.started = true;
        animation.lastExecution = time;
    }
    

    if (time >= animation.interval + animation.lastExecution) 
    {
        for (uint8_t i = 0; i < arrayLength; i++) {

            stripes[i].fill(stripes[i].Color(r, g, b));
            stripes[i].show();
        }
        
        animation.blink = true;
        animation.cont = 0;
        animation.lastExecution = time;
    }
    

    if (time >= animation.intervalBlinks + animation.lastExecutionBlinks && animation.blink) 
    {
        for (uint8_t i = 0; i < arrayLength; i++) {

            stripes[i].setBrightness(100);
            stripes[i].show();
        }

        delay(10);

        for (uint8_t i = 0; i < arrayLength; i++) {

            stripes[i].setBrightness(5);
            stripes[i].show();
        }

        animation.cont++;
        animation.lastExecutionBlinks = time;
        if (animation.cont == 2) {

            animation.blink = false;
        }
    }
}

void algoritmo(uint32_t &time) {
    
    if (walkers == 0) {

        walkersOpenTimeout.lastExecution = time;
        changeState(OPENCARS, time);
        printLcd(timesLCD, time, 0, false);
    } else if (cars == 0) {

        
        changeState(OPENWALKERS, time);
        printLcd(timesLCD, time, 0, true);
    } else {

        calcTimeout(walkersOpenTimeout.interval, time);
        if (time >= walkersOpenTimeout.interval + walkersOpenTimeout.lastExecution) {
            
            changeState(OPENWALKERS, time);
        } else {

            changeState(OPENCARS, time);
        }

        printLcd(timesLCD, time, (walkersOpenTimeout.interval + walkersOpenTimeout.lastExecution - time) / 1000, false);
    }
    
}

void changeState(uint8_t _state, uint32_t time) {

    if (state != _state) {

        switch (_state) {

        case OPENCARS:
            
            state = CHANGESEMAFORO;
            animations[0].started = false;
            
            break;
        case OPENWALKERS:

            state = CHANGESEMAFORO;
            animations[1].started = false;
            timeWalkersToCross.started = false;
            break;
        default:
            break;
        }

        turnOffLights();
    }
}

void calcTimeout(uint32_t &interval, uint32_t &time) {

    uint32_t newInterval = 20000;

    if (walkers < 6) {
        
        uint8_t percentaje = walkers * 10;
        newInterval = TIMEOUTWALKERS / 100 * (100 - percentaje);
        //Serial.println(newInterval);
        
    }

    if (newInterval < interval) {

        interval = newInterval;
        walkersOpenTimeout.lastExecution = time;
    }
}

void checkTimeoutOpenWalkers(uint32_t &time) {

    if (!timeWalkersToCross.started) {
        
        timeWalkersToCross.started = true;
        timeWalkersToCross.lastExecution = time;
    }
    
    if (time >= timeWalkersToCross.interval + timeWalkersToCross.lastExecution)
    {
        changeState(OPENCARS, time);
        walkers = 0;
    }

    printLcd(timesLCD, time, (timeWalkersToCross.interval + timeWalkersToCross.lastExecution - time) / 1000, true);
}

void turnOffLights() {

    for (uint8_t i = 0; i < 2; i++) {
        waitWalkerStripes[i].clear();
        waitWalkerStripes[i].show();
    }

    for (uint8_t i = 0; i < 1; i++) {
        walkerAssistantStripes[i].clear();
        walkerAssistantStripes[i].show();
    }
}

void animateSemaforo(uint32_t &time) {

    if (semaforo.direction) {

        delay(2000);
        turnOffSemaforo();
        digitalWrite(semaforo.CAR_GREEN_PIN, HIGH);
        digitalWrite(semaforo.WALKER_RED_PIN, HIGH);
        semaforo.animationState = 1;
        semaforo.direction = !semaforo.direction;
        state = OPENCARS;
        Serial.println("Verde");

        //TODO: A quitar
    } else if (!semaforo.started) {

        animationSemaforo.lastExecution = time;
        semaforo.started = true;
    } else if (time >= animationSemaforo.interval + animationSemaforo.lastExecution) {

        turnOffSemaforo();
        switch (semaforo.animationState)
        {
        case 0:
            digitalWrite(semaforo.CAR_RED_PIN, HIGH);
            digitalWrite(semaforo.WALKER_GREEN_PIN, HIGH);
            semaforo.started = false;
            semaforo.direction = !semaforo.direction;
            state = OPENWALKERS;
            Serial.println("Rojo");
            //TODO: A quitar
            delay(2000);
            break;

        case 1:
            digitalWrite(semaforo.CAR_YELLOW_PIN, HIGH);
            Serial.println("Amarillo");
            break;
        
        default:
            break;
        }

        semaforo.animationState--;
        animationSemaforo.lastExecution = time;
    }
}

void turnOffSemaforo() {

    digitalWrite(semaforo.CAR_RED_PIN, LOW);
    digitalWrite(semaforo.CAR_YELLOW_PIN, LOW);
    digitalWrite(semaforo.CAR_GREEN_PIN, LOW);
    digitalWrite(semaforo.WALKER_RED_PIN, LOW);
    digitalWrite(semaforo.WALKER_GREEN_PIN, LOW);
    
}

void addWalkers(uint32_t &time) {

    if (time >= addWalkersTimer.interval + addWalkersTimer.lastExecution && digitalRead(WALKERBUTTON) && walkers < 9) {
        
        walkers++;
        addWalkersTimer.lastExecution = time;
    }
    
}

void checkStopCars(uint32_t &time) {
    
    if (time >= stopCars.interval + stopCars.lastExecution && digitalRead(STOPCARSBUTTON)) {
        
        if (cars == 0) {
            
            cars = random(1, 5);
        } else {

            cars = 0;
        }

        stopCars.lastExecution = time;
    }
}