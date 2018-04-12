/* Included Libraries */
#include <Enums.h>
#include <ChargeController.h>
#include <ValveController.h>
#include <SimpleTimer.h>
#include <Thermostat.h>
#include <ActivityMonitor.h>
#include <TimerManager.h>
/*
  Description:


*/



/**
   Pin Declarations
*/
byte MD10C_DIR_PIN = 4;
byte MD10C_PWM_PIN = 5;
byte MD10C_MOSFET_PIN = 6;
byte THERMOSTAT_PIN = 2;
byte CHARGECONTROLLER_RELAY_PIN = 12;
byte CHARGECONTROLLER_INPUT_PIN = A4;
byte OVERRIDE_ON_PIN = 7;
byte OVERRIDE_OFF_PIN = 8;
byte ACTIVITYMONITOR_INPUT_PIN = 3;
byte INDICATOR_R_PIN = 9;
byte INDICATOR_G_PIN = 10;
byte INDICATOR_B_PIN = 11;

//
SimpleTimer timer;
TimerManager timerManager(timer);

//
Thermostat thermostat(THERMOSTAT_PIN);
ValveController valveController(MD10C_DIR_PIN, MD10C_PWM_PIN, MD10C_MOSFET_PIN);
ChargeController chargeController(CHARGECONTROLLER_RELAY_PIN, CHARGECONTROLLER_INPUT_PIN);
ActivityMonitor activityMonitor(ACTIVITYMONITOR_INPUT_PIN);

class MainClass {
  private:
    FireplaceStatus fireplaceStatus;
  public:
    MainClass() {
      this->fireplaceStatus = FireplaceStatus::OFF;
    }

    void run() {

    }

};

MainClass main;

void runMain() {
  main.run();
}

void runActivityMonitor() {
  activityMonitor.run();
}

void runThermostat(){
  thermostat.run();
}

void runChargeController(){
  chargeController.run();
}


void setup() {
  Serial.begin(9600);

  // Output Pins
  pinMode(MD10C_DIR_PIN, OUTPUT);
  pinMode(MD10C_PWM_PIN, OUTPUT);
  pinMode(MD10C_MOSFET_PIN, OUTPUT);
  pinMode(CHARGECONTROLLER_RELAY_PIN, OUTPUT);

  // Input Pins
  pinMode(THERMOSTAT_PIN, INPUT);
  pinMode(CHARGECONTROLLER_INPUT_PIN, INPUT);
  pinMode(OVERRIDE_ON_PIN, INPUT);
  pinMode(OVERRIDE_OFF_PIN, INPUT);
  pinMode(ACTIVITYMONITOR_INPUT_PIN, INPUT);

  runChargeController();
  runThermostat();

  // Setup Timers
  timerManager.addTimer((int)TimerType::MAIN_CLASS, timer.setInterval(1000, runMain));
  timerManager.addTimer((int)TimerType::THERMOSTAT, timer.setInterval(1000, runThermostat));
  timerManager.addTimer((int)TimerType::ACTIVITY_MONITOR, timer.setInterval(1000, runActivityMonitor));
  timerManager.addTimer((int)TimerType::CHARGECONTROLLER, timer.setInterval(60000, runChargeController));
}

void loop() {
  timerManager.run();
  timer.run();
}
