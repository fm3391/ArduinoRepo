/*
 * ValveController.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ValveController.h"
#include "ValveControllerStateEnum.h"

ValveController::ValveController(int dirPin,int pwmPin,int mosfetPin):md10c(dirPin,pwmPin), mosfet(mosfetPin){	this->state = ValveControllerStateEnum::CLOSED;}
ValveControllerStateEnum ValveController::getState(){
	return state;
}
void ValveController::openValve(int battVoltage) { 
Serial.println("Opening Valve");
mosfet.setGate(HIGH);
md10c.setDir(1);
int pwmVal = (int)((maxVoltageOut/battVoltage) * maxPwmOut);
Serial.println(String(pwmVal));
md10c.setPwm((int) pwmVal);
delay(1000);
md10c.setPwm(0);
mosfet.setGate(LOW);
this->state = ValveControllerStateEnum::OPEN;}

void ValveController::closeValve(int battVoltage) {
Serial.println("Closing Valve");mosfet.setGate(HIGH);
md10c.setDir(0);
int pwmVal = (int)((maxVoltageOut/battVoltage) * maxPwmOut);
Serial.println(String(pwmVal));
md10c.setPwm((int) pwmVal);
delay(1000);
md10c.setPwm(0);
mosfet.setGate(LOW);
this->state = ValveControllerStateEnum::CLOSED;}