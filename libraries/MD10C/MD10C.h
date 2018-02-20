/*
 * MD10C.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef MD10C_h
#define MD10C_h

#include "Arduino.h"

class MD10C {
private:
	byte dirPin;
	byte pwmPin;

public:
	MD10C(byte dirPinNumber, byte pwmPinNumber);
	void setDir(int i);
	void setPwm(int i);
};

#endif
