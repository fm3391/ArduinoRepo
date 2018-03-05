/*
 * MD10C.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "MD10C.h"

MD10C::MD10C(byte dirPinNumber, byte pwmPinNumber) {
	this->dirPin = dirPinNumber;
	this->pwmPin = pwmPinNumber;
}

void MD10C::setDir(int i) {
	digitalWrite(dirPin, i);
}

void MD10C::setPwm(int i) {
	analogWrite(pwmPin, i);
}

