/*
 * ChargeController.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ChargeController.h"
#include "Enums.h"

ChargeController::ChargeController(int relayCtrlPin, int batteryInputPin) {
	this->relayCtrlPin = relayCtrlPin;
	this->batteryInputPin = batteryInputPin;
	this->voltage = 0.00;
}

void ChargeController::updateVoltage(){
	int inputVal = analogRead(batteryInputPin);
	this->voltage = (float)inputVal/(float)1023 * nominalVoltage;
	//Serial.println(String(this->voltage));
}

BatteryStatus ChargeController::getBatteryStatus(){
	BatteryStatus status;
	if(voltage > 11.00){
		status = BatteryStatus::FULL;
	}else if(voltage > 10.00){
		status = BatteryStatus::GOOD;
	}else if(voltage > 9.60){
		status = BatteryStatus::WARNING;
	}else{
		status = BatteryStatus::LOW_BATT;
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
			if(this->voltage < nominalVoltage){
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
		if(this->voltage < minVoltage){
			isCharging = true;
			enableCharging();
			chargeCounter = 0;
		}
		
	}
}