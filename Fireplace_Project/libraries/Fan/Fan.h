/*
 * Fan.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef Fan_h
#define Fan_h

#include "Arduino.h"

class Fan {
  private:
    int relayPin;
    bool isRunning;
		
  public:
    Fan(int relayPin);
    bool fanIsRunning();
    void start();
    void stop();
};

#endif
