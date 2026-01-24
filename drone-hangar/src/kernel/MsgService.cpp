#include "kernel/MsgService.h"

MsgServiceClass MsgService;

MsgServiceClass::MsgServiceClass(){
    currentMsg = NULL;
    msgAvailable = false;
}

void MsgServiceClass::init(){
    Serial.begin(9600);
    content.reserve(256);
    content = "";
}

void MsgServiceClass::sendMsg(const String& msg){
    Serial.println(msg);
}

bool MsgServiceClass::isMsgAvailable(){
    checkSerial();
    return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
    if (msgAvailable){
        Msg* msg = currentMsg;
        msgAvailable = false;
        content = "";
        return msg;
    } else {
        return NULL;
    }
}

void MsgServiceClass::checkSerial() {
    while (Serial.available()) {
        char ch = (char) Serial.read();
        if (ch == '\n'){
            if (currentMsg != NULL) {
                delete currentMsg;
            }
            currentMsg = new Msg(content);
            msgAvailable = true;
        } else {
            content += ch;
        }
    }
}