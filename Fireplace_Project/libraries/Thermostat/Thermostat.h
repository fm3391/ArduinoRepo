/*
  Thermostat.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Thermostat_h
#define Thermostat_h

#include "Arduino.h"

class Thermostat {
  private:
		int heatPin;
    int coolPin;
    int fanPin;

  public:
		Thermostat(int heatPin, int coolPin, int fanPin);
		bool isCooling();
		bool isHeating();
		bool isFanRunning();

};

#endif