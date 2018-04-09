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
		byte heatPin;
		bool is_Heating;

  public:
		Thermostat(byte heatPinIn);
		bool isHeating();
		void run();
};

#endif