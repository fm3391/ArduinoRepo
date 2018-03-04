/*
  Thermostat.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Enums_h
#define Enums_h

enum class ThermostatMode {
  OFF, HEAT, COOL
};

enum class ThermostatFanState {
  OFF, RUNNING
};

enum class FireplaceStatus {
  OFF, RUNNING, UNKNOWN = 99
};

enum class BatteryStatus {
  LOW_BATT = 0, WARNING = 1, GOOD = 2, FULL = 3, UNKNOWN = 99
};

enum class FireplaceState {
  OFF = 0, RUNNING = 1
};

enum class SystemMode {
  LOW_BATT = 0, OVERRIDE = 1, NORMAL = 2
};

enum class MessageType {
  REQ = 0, CMD =1 , INFO = 2, UNKNOWN = 99
};

enum class MessageSpecifier {
  BATT = 0, FIRE = 1, UNKNOWN = 99
};

enum class MessageCmd {
  FIRE_OFF = 0, FIRE_ON = 1
};


#endif