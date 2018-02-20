/*
 * Mosfet.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef Mosfet_h
#define Mosfet_h

#include "Arduino.h"

class Mosfet {
  private:
    int gatePin;

  public:
    Mosfet(int gatePinNumber);
    void setGate(int i);
};

#endif
