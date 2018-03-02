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
	this->voltage = 0.00;
	batteryLevel = 0;
}

void ChargeController::updateVoltage(){
	int inputVal = analogRead(batteryInputPin);
	voltage = (float)inputVal/(float)1023 * nominalVoltage;
	if(voltage > 11.00){
		batteryLevel = 3;
	}else if(voltage > 10.00){
		batteryLevel = 2;
	}else if(voltage > 9.60){
		batteryLevel = 1;
	}else{
		batteryLevel = 0;
	}
	//Serial.println(String(this->voltage));
}

int ChargeController::getBatteryLevel(){
	return batteryLevel;
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