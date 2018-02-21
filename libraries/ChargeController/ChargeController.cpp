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
	int inputVal = analogRead(batteryInputPin);
	this->voltage = inputVal * this->multiplier;
	Serial.println(String(this->voltage) + " / " + String(inputVal));
}

float ChargeController::getVoltage(){
	return voltage;
}

void ChargeController::run(){
	// Checks the current state of charging
	if(isCharging){
		if(chargeCounter == chargeCounterMax){
			disableCharging();
			updateVoltage();
			if(this->voltage < battMaxVoltage){
				enableCharging();
				chargeCounter = 5;
			}else{
				isCharging = false;
			}
		}else{
			chargeCounter++;
		}

	}else{
		// Is not charging
		// Check to see if the battery needs to be charging
		updateVoltage();
		if(this->voltage < battMinVoltage){
			isCharging = true;
			enableCharging();
			chargeCounter = 0;
		}
		
	}
}