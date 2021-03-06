/*
 * ValveController.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef ValveController_h
#define ValveController_h

#include "Arduino.h"
#include "Mosfet.h"
#include "MD10C.h"
#include "Enums.h"

class ValveController {
private:
  const float maxVoltageOut = 6.0;
  const float maxPwmOut = 254.0;
  const int delayTime = 750;
  MD10C md10c;
  Mosfet mosfet;
  void cycle();

public:
  ValveController(int dirPin,int pwmPin,int mosfetPin);
  void openValve(int battVoltage);
  void closeValve(int battVoltage);
};

#endif
