#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class Msg {
private:
  String content;

public:
  Msg(String content){
    this->content = content;
  }

  String getContent(){
    return content;
  }
};

class MsgServiceClass {
public:
    MsgServiceClass();
    void init();

    bool isMsgAvailable();

    Msg* receiveMsg();

    void sendMsg(const String& msg);

    void checkSerial();
private:
    String content;
    Msg* currentMsg;
    bool msgAvailable;
};

extern MsgServiceClass MsgService;

#endif