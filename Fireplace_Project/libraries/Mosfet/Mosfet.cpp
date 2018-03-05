/*
 * Mosfet.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "Mosfet.h"

Mosfet::Mosfet(int gatePinNumber) {
  gatePin = gatePinNumber;
}

void Mosfet::setGate(int i) {
  if (i == 1) {
    digitalWrite(gatePin, HIGH);
  } else if (i == 0) {
    digitalWrite(gatePin, LOW);
  }
}


