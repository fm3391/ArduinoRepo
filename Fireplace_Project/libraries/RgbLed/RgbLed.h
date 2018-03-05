/*
 * RgbLed.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef RgbLed_h
#define RgbLed_h

#include "Arduino.h"

class RgbLed {
  private:
    byte rPin;
    byte gPin;
    byte bPin;

  public:
    RgbLed(byte rPinNum, byte gPinNum, byte bPinNum);
    void setColor(byte rVal, byte gVal, byte bVal);
	void setOff();
};

#endif
