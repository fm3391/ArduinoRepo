/*
 * ValveController.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ValveController.h"
#include "Enums.h"

ValveController::ValveController(int dirPin,int pwmPin,int mosfetPin):md10c(dirPin,pwmPin), mosfet(mosfetPin){	}

void ValveController::openValve(int battVoltage) { 
mosfet.setGate(HIGH);
md10c.setDir(1);
int pwmVal = (int)((maxVoltageOut/battVoltage) * maxPwmOut);
md10c.setPwm((int) pwmVal);
delay(delayTime);
md10c.setPwm(0);
mosfet.setGate(LOW);}

void ValveController::closeValve(int battVoltage) {mosfet.setGate(HIGH);
md10c.setDir(0);
int pwmVal = (int)((maxVoltageOut/battVoltage) * maxPwmOut);
md10c.setPwm((int) pwmVal);
delay(delayTime);
md10c.setPwm(0);
mosfet.setGate(LOW);}