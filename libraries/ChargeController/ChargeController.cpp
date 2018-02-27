/*
 * ChargeController.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ChargeController.h"
#include "BatteryStatus.h"

ChargeController::ChargeController(int relayCtrlPin, int batteryInputPin) {
	this->relayCtrlPin = relayCtrlPin;
	this->batteryInputPin = batteryInputPin;
	updateVoltage();
}

void ChargeController::updateVoltage(){
	int inputVal = analogRead(batteryInputPin);
	this->voltage = (float)inputVal/(float)1023 * battMaxVoltage;
}

BatteryStatus ChargeController::getBatteryStatus(){
	BatteryStatus status;
	if(voltage > 11.00){
		status = BatteryStatus::FULL;
	}else if(voltage > 9.60){
		status = BatteryStatus::GOOD;
	}else if(voltage > 8.50){
		status = BatteryStatus::WARNING;
	}else{
		status = BatteryStatus::DEPLETED;
	}
	return status;
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