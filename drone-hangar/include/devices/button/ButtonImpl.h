#ifndef __BUTTON_IMPL__
#define __BUTTON_IMPL__

#include "Button.h"

class ButtonImpl : public Button {
private:
  int pin;
  unsigned long lastDebounceTime;
  int lastButtonState;
  int buttonState;
  static const unsigned long DEBOUNCE_DELAY = 50; // milliseconds

public:
  ButtonImpl(int pin);
  bool isPressed();
};

#endif