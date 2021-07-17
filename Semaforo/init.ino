void initLedStripes() {

    for (uint8_t i = 0; i < 2; i++)
    {
        waitWalkerStripes[i].begin();
        waitWalkerStripes[i].clear();
        waitWalkerStripes[i].setBrightness(5);
        waitWalkerStripes[i].fill(waitWalkerStripes[i].Color(0, 0, 255));
        waitWalkerStripes[i].show();
    }

    for (uint8_t i = 0; i < 1; i++)
    {
        walkerAssistantStripes[i].begin();
        walkerAssistantStripes[i].clear();
        walkerAssistantStripes[i].setBrightness(5);
        walkerAssistantStripes[i].fill(walkerAssistantStripes[i].Color(0, 0, 255));
        walkerAssistantStripes[i].show();
    }

    walkersOpenTimeout.interval = TIMEOUTWALKERS;
    timeWalkersToCross.interval = TIMETOCROSS;
    delay(1000);
    turnOffLights();
}

void initSemaforo() {

    semaforo.CAR_RED_PIN = 3;
    semaforo.CAR_YELLOW_PIN = 4;
    semaforo.CAR_GREEN_PIN = 5;
    semaforo.WALKER_RED_PIN = 6;
    semaforo.WALKER_RED_PIN = 7;

    pinMode(semaforo.CAR_RED_PIN, OUTPUT);
    pinMode(semaforo.CAR_YELLOW_PIN, OUTPUT);
    pinMode(semaforo.CAR_GREEN_PIN, OUTPUT);
    pinMode(semaforo.WALKER_RED_PIN, OUTPUT);
    pinMode(semaforo.WALKER_RED_PIN, OUTPUT);
    semaforo.animationState = 1;
    semaforo.direction = true;
    animationSemaforo.interval = 1000;
}

void initAnimations() {

    animations[0].interval = 4000;
    animations[0].intervalBlinks = 50;
    animations[0].cont = 0;

    animations[1].interval = 1000;
    animations[1].intervalBlinks = 100;
    animations[1].cont = 0;
}

void initLCD() {

    timesLCD.setBright(10);
    timesLCD.setDigitLimit(8);
    infoRoad.setBright(10);
    infoRoad.setDigitLimit(8);
    timesLCDTimer.interval = 1000;
    infoRoadLCDTimer.interval = 1000;
}

void initButtons() {

    pinMode(WALKERBUTTON, INPUT);
    pinMode(STOPCARSBUTTON, INPUT);

    addWalkersTimer.interval = 1000;
    stopCars.interval = 1000;
}