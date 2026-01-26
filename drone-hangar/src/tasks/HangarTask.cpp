#include "tasks/HangarTask.h"
#include <Arduino.h> // Necessario per millis()

HangarTask::HangarTask(LcdI2C* lcd, Led* l1, Button* resetButton, Context * context)
    :lcd(lcd), l1(l1), resetButton(resetButton), context(context) {
    lcd->turnOn();
    // Inizializzazione variabili FSM
    setState(DRONE_INSIDE);
    takeoffConfirmationTimer = 0;
    landingConfirmationTimer = 0;
    updateDisplay();
}

void HangarTask::tick() {
    switch (state) {
    case DRONE_INSIDE:
        if (checkAndSetJustEntered()) {
            l1->switchOn();
            context->confirmDroneInside();
        }
        // Logica di transizione
        if (context->isTakeOffCommandReceived()) {
            setState(TAKE_OFF);
        }
        break;

    case TAKE_OFF:
        if (checkAndSetJustEntered()) {
            l1->switchOff();
        }
        
        // Logica di transizione
        if(context->isDroneOut()) {
            setState(DRONE_OUT);
        }
        break;

    case DRONE_OUT:
        if (checkAndSetJustEntered()) {
            l1->switchOff();
            context->confirmDroneOut();
        }
        
        // Logica di transizione
        if (context->isLandingCommandReceived()) {
            setState(LANDING);
        }
        break;

    case LANDING:
        if (checkAndSetJustEntered()) {
            l1->switchOff();
        }
        
        // Logica di transizione
        if(context->isDroneInside()) {
            setState(DRONE_INSIDE);
        }
        break;

    case PRE_ALARM:
        if (checkAndSetJustEntered()) {
            // Logica pre-allarme
        }
        break;

    case ALARM:
        if (checkAndSetJustEntered()) {
            // Logica allarme
        }
        break;
    }
}

void HangarTask::updateDisplay() {
    lcd->clear();
    lcd->setCursor(0, 0); // Posizionati all'inizio

    String message;
    switch (state) {
        case DRONE_INSIDE:
            message = "DRONE INSIDE";
            break;
        case TAKE_OFF:
            message = "TAKE OFF";
            break;
        case DRONE_OUT:
            message = "DRONE OUT";
            break;
        case LANDING:
            message = "LANDING";
            break;
        case PRE_ALARM:
            message = "PRE-ALARM";
            break;
        case ALARM:
            message = "ALARM";
            break;
    }
    lcd->print(message);
}

bool HangarTask::checkAndSetJustEntered() {
    bool result = justEntered;
    if (justEntered) {
        // Appena entrati in un nuovo stato, aggiorniamo il display automaticamente
        updateDisplay(); 
        justEntered = false;
    }
    return result;
}

// CORREZIONE CRUCIALE QUI SOTTO
void HangarTask::setState(HangarState newState) {
    this->state = newState;
    this->stateTimestamp = millis(); // Resetta il tempo
    this->justEntered = true;        // Attiva il flag di ingresso per il prossimo tick
}