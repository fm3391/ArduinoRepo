/*
 * RgbLed.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "RgbLed.h"


RgbLed::RgbLed(byte rPinNum, byte gPinNum, byte bPinNum) {
  rPin = rPinNum;
  gPin = gPinNum;
  bPin = bPinNum;
}


void RgbLed::setColor(byte rVal, byte gVal, byte bVal) {
  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);
}

void RgbLed::setOff(){
	analogWrite(rPin, 0);
	analogWrite(gPin, 0);
	analogWrite(bPin, 0);
}