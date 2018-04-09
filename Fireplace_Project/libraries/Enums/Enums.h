/*
  Enums.h - 
*/
#ifndef Enums_h
#define Enums_h


enum class TimerType{
	MAIN_CLASS = 0,
	THERMOSTAT = 1,
	ACTIVITY_MONITOR = 2,
	CHARGECONTROLLER = 3
};
/*

*/
enum class FireplaceStatus {
  OFF = 0, RUNNING = 1
};

/*

*/
enum class BatteryStatus {
  LOW_BATT = 0, WARNING = 1, GOOD = 2, FULL = 3, UNKNOWN = 99
};

/*

*/
enum class SystemMode {
  OVERRIDE = 0, NORMAL = 1
};
#endif