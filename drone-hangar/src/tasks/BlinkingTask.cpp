#include "tasks/BlinkingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "tasks/BlinkingTask.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext):
    pContext(pContext), pLed(pLed){
    setState(IDLE);
}

void BlinkingTask::init(int period) {
    PeriodicTask::init(period);
}

void BlinkingTask::tick()
{
    switch (state){
    case IDLE: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] IDLE"));
        }
        if (pContext->isDroneInside()){
            setState(OFF);
        }
        break;
    }
    case OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] OFF"));
        }
        if (pContext->isStopped()){
            setState(IDLE);
        } else {
             setState(ON);
        }
        break;
    }
    case ON: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOn();
            Logger.log(F("[BT] ON"));
        }
        if (pContext->isStopped()){ //TODO
            setState(IDLE);
        } else {
            setState(OFF);
        }
        break;
    }
    }
}

void BlinkingTask::setState(BlinkingState s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long BlinkingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool BlinkingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}