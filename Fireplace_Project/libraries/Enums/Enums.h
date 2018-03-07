/*
  Enums.h - 
*/
#ifndef Enums_h
#define Enums_h
/*

*/
enum class ThermostatMode {
  OFF = 0, HEAT = 1, COOL = 2
};
/*

*/
enum class ThermostatFanState {
  OFF = 0, RUNNING = 1
};
/*

*/
enum class FireplaceStatus {
  OFF = 0, RUNNING = 1, UNKNOWN = 99
};
/*

*/
enum class ChargingStatus {
  NOT_CHARGING = 0, CHARGING = 1, UNKNOWN = 99
};
/*

*/
enum class BatteryStatus {
  LOW_BATT = 0, WARNING = 1, GOOD = 2, FULL = 3, UNKNOWN = 99
};
/*

*/
enum class BluetoothState {
  DISABLED = 0, ENABLED = 1
};
/*

*/
enum class SystemMode {
  OVERRIDE = 0, NORMAL = 1
};
/*

*/
enum class MessageType {
  REQ = 0, CMD =1 , INFO = 2, UNKNOWN = 99
};
/*

*/
enum class MessageSpecifier {
  BATT = 0, FIRE = 1, CHARGE = 2, UNKNOWN = 99
};
/*

*/
enum class MessageCmd {
  FIRE_OFF = 0, FIRE_ON = 1
};
#endif