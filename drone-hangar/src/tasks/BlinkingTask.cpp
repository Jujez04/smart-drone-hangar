#include "tasks/BlinkingTask.h"
#include "kernel/Logger.h"
#include "config.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext)
    : pLed(pLed), pContext(pContext) {
    setState(IDLE);
}

void BlinkingTask::init(int period) {
    PeriodicTask::init(period);
}

void BlinkingTask::tick() {

    bool systemBusy = (pContext->isTakeOffCommandReceived() || pContext->isLandingCommandReceived()) && !(pContext->isPreAlarm() || pContext->isAlarm());
    switch (state) {
        case IDLE: {
            if (checkAndSetJustEntered()) {
                pLed->switchOff();
            }
            if (systemBusy) {
                setState(ON);
            }
            break;
        }

        case ON: {
            if (checkAndSetJustEntered()) {
                pLed->switchOn();
            }

            if (elapsedTimeInState() >= BLINK_PERIOD) {
                setState(OFF);
            }
            if (!systemBusy) {
                setState(IDLE);
            }
            break;
        }

        case OFF: {
            if (checkAndSetJustEntered()) {
                pLed->switchOff();
            }

            if (elapsedTimeInState() >= BLINK_PERIOD) {
                setState(ON);
            }
            if (!systemBusy) {
                setState(IDLE);
            }
            break;
        }
    }
}

void BlinkingTask::setState(BlinkingState s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long BlinkingTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool BlinkingTask::checkAndSetJustEntered() {
    bool result = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return result;
}