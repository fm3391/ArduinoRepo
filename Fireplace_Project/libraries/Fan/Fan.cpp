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

bool Fan::fanIsRunning() {
	return this->isRunning;
}

void Fan::start() {
	digitalWrite(this->relayPin, HIGH);
	isRunning = true;
}

void Fan::stop() {
	digitalWrite(this->relayPin, LOW);
	isRunning = false;
}