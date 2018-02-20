/*
 * ChargeController.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ChargeController.h"


ChargeController::ChargeController(int relayCtrlPin, int batteryInputPin) {
	this->relayCtrlPin = relayCtrlPin;
	this->batteryInputPin = batteryInputPin;
	updateVoltage();
	
}

void ChargeController::updateVoltage(){
	this->voltage = (analogRead(batteryInputPin) * 0.01635);
}

float ChargeController::getVoltage(){
	return voltage;
}

void ChargeController::run(){
	updateVoltage();
	// Checks the current state of charging
	if(isCharging){
		if(voltage >= battMaxVoltage){
			disableCharging();
			isCharging = false;
		}

	}else{
		// Is not charging
		// Check to see if the battery needs to be charging
		
		if(voltage < battMinVoltage){
			enableCharging();
			isCharging = true;
		}
		
	}
}