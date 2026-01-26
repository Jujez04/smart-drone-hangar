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

void BlinkingTask::tick() {
    switch (state) {
        case IDLE:
            if (checkAndSetJustEntered()) pLed->switchOff();
            if (pContext->isTakeOffCommandReceived()) setState(ON);
            break;

        case OFF:
            if (checkAndSetJustEntered()) pLed->switchOff();
            // CORREZIONE: Passa a ON dopo il tempo stabilito
            if (elapsedTimeInState() >= BLINK_PERIOD) setState(ON);
            
            // Uscita
            if (pContext->isDroneOut() || (pContext->isDroneInside() && !pContext->isTakeOffCommandReceived())) 
                setState(IDLE);
            break;

        case ON:
            if (checkAndSetJustEntered()) pLed->switchOn();
            // CORREZIONE: Passa a OFF dopo il tempo stabilito (questo mancava!)
            if (elapsedTimeInState() >= BLINK_PERIOD) setState(OFF);
            
            // Uscita immediata se il drone è fuori
            if (pContext->isDroneOut()) setState(IDLE);
            break;
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