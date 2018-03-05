/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef FanController_h
#define FanController_h

#include "Arduino.h"

class FanController {
  private:
    int fanRelayPin;

  public:
    FanController(int relayPin);
    void startFan();
    void stopFan();
};

#endif