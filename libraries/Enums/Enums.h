/*
  Thermostat.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Enums_h
#define Enums_h

enum ThermostatMode {
  OFF, HEAT, COOL
};

enum class ThermostatFanState {
  OFF, RUNNING
};

enum class FireplaceStatus {
  OFF, RUNNING, UNKNOWN
};

enum class BatteryStatus {
  LOW_BATT , WARNING , GOOD, FULL, UNKNOWN
};



#endif