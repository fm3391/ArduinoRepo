#include "Thermostat.h"

Thermostat::Thermostat(int heatPin, int coolPin, int fanPin) {
		this->heatPin = heatPin;
		this->coolPin = coolPin;
		this->fanPin = fanPin;
}

bool Thermostat::isCooling() {
	bool isCooling = false;
	if (digitalRead(coolPin) == HIGH) {
		isCooling = true;
	}
	return isCooling;
}

bool Thermostat::isHeating() {
	bool isHeating = false;
	if (digitalRead(heatPin) == HIGH) {
		isHeating = true;
	}
	return isHeating;
}

bool Thermostat::isFanRunning() {
	bool isFanRunning = false;
	if (digitalRead(fanPin) == HIGH) {
		isFanRunning = true;
	}
	return isFanRunning;
}