#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <OccupancyMonitor.h>
#include <Thermostat.h>
#include <FanController.h>

const int fanRelayPin = 0;
const int occupancyInputPin = 0;
const int btStatePin = 0;

SimpleTimer timer;
OccupancyMonitor occupancyMonitor;
MessageManager messageManager;

/*
 * Check if the bluetooth module is connected
 */
bool isConnected(){
  bool isConnected = false;
  if(digitalRead(btStatePin) == HIGH){
    isConnected = true;
  }
  return isConnected;
}

/*
 * 
 */
class RoomManager {
  private:
    bool isInit = false;
    Thermostat thermostat;
    ThermostatMode thermostatMode;
    ThermostatFanState thermostatFanState;
    BatteryStatus batteryStatus = BatteryStatus::UNKNOWN;
    FireplaceStatus fireplaceStatus = FireplaceStatus::UNKNOWN;
    FanController fanController;
    MessageManager *messageManager;


    void updateThermostatMode() {
      if (thermostat.isCooling() || thermostat.isHeating()) {
        if (thermostat.isCooling() && thermostatMode != ThermostatMode::COOL) {
          thermostatMode = ThermostatMode::COOL;
        } else if (thermostat.isHeating() && thermostatMode != ThermostatMode::HEAT) {
          thermostatMode = ThermostatMode::HEAT;
        }
      } else if (thermostatMode != ThermostatMode::OFF) {
        thermostatMode = ThermostatMode::OFF;
      }
    }

    void updateThermostatFanState(){
      if(thermostat.isFanRunning() && thermostatFanState != ThermostatFanState::RUNNING){
        thermostatFanState = ThermostatFanState::RUNNING;
      }else if(!thermostat.isFanRunning() && thermostatFanState == ThermostatFanState::RUNNING){
        thermostatFanState = ThermostatFanState::OFF;
      }
    }

  public:
    RoomManager(int coolPinIn, int heatPinIn, int fanPinIn, int fanRelayPinIn, MessageManager &messageManagerIn)
    : thermostat(coolPinIn, heatPinIn, fanPinIn),fanController(fanRelayPinIn) {
      this->messageManager = &messageManagerIn;
    }
    void run() {
      if (!isInit) {
        if (isConnected()) {
          // Request information from FireplaceController
          messageManager->addOutboundMsg("REQ:ALL");
          delay(1000);
        }
        updateThermostatMode();
        updateThermostatFanState();
        isInit = true;
      }

      

      




    }

};




void activityDetected(){
  occupancyMonitor.activityDetected();
}

void runOccupancyMonitor(){
  occupancyMonitor.run();
}

void runDataCollection(){
  
}

void runMessageManager(){
  messageManager.run();
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(occupancyInputPin), activityDetected, RISING);
  timer.setInterval(1000, runOccupancyMonitor);
  timer.setInterval(1000, runMessageManager);
  timer.setInterval(5000, runDataCollection);
}

void loop() {
  timer.run();
}
