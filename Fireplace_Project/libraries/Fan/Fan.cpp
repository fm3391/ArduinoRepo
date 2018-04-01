/*
 * Fac.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "Fan.h"

Fan::Fan(int relayPin) {
	this->relayPin = relayPin;
}

bool Fan::isRunning() {
	return this->is_Running;
}

void Fan::start() {
	//digitalWrite(this->relayPin, HIGH);
	Serial.println("FAN_RUNNING");
	is_Running = true;
}

void Fan::stop() {
	//digitalWrite(this->relayPin, LOW);
	Serial.println("FAN_STOPPED");
	is_Running = false;
}