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
    bool is_Running;
		
  public:
    Fan(int relayPin);
    bool isRunning();
    void start();
    void stop();
};

#endif
