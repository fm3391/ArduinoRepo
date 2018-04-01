// Libraries
#include <Fan.h>
#include <ActivityMonitor.h>
#include <Thermostat.h>
#include <BluetoothController.h>
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <SimpleQueue.h>
#include <QList.h>
#include <TimerManager.h>

// Input pins
const int btStatePin = 6;
const int heatPin = 7;
const int coolPin = 8;
const int pirPin = 5;
const int fanPin = 4;

// Indicator pins
const int statusIndicator_RPin = 9;
const int statusIndicator_GPin = 10;
const int statusIndicator_BPin = 11;
const int heartBtLedPin = 12;

const String SEPERATOR = ":";

const String REQ_BATT_STATUS_MSG = String((int)MessageType::REQ) + SEPERATOR + String((int)MessageSpecifier::BATT);
const String REQ_FIRE_STATUS_MSG = String((int)MessageType::REQ) + SEPERATOR + String((int)MessageSpecifier::FIRE);

const String CMD_FIRE_ON_MSG =  String((int)MessageType::CMD) + SEPERATOR +
                                String((int)MessageSpecifier::FIRE) + SEPERATOR +
                                String((int)MessageCmd::FIRE_ON);

const String CMD_FIRE_OFF_MSG = String((int)MessageType::CMD) + SEPERATOR +
                                String((int)MessageSpecifier::FIRE) + SEPERATOR +
                                String((int)MessageCmd::FIRE_OFF);



SimpleTimer timer;
MessageManager messageManager;
BluetoothController btController(btStatePin, -99);
Thermostat thermostat(heatPin, coolPin);
ActivityMonitor activityMonitor(pirPin);
Fan fan(fanPin);
TimerManager timerMngr(timer);


void runTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

class MainApp {
  private:
    bool isInit = false;
    FireplaceStatus fireStatus = FireplaceStatus::UNKNOWN;
    BatteryStatus battStatus = BatteryStatus::UNKNOWN;
    int battUpdateCounter = 0;
    const int battUpdateCounterMax = 20;
    int fireUpdateCounter = 0;
    const int fireUpdateCounterMax = 10;

    //
    void runInitialize() {
      timerMngr.disableAll();
      int cntr = 0;
      requestFireStatus();
      requestBattStatus();
      while (fireStatus == FireplaceStatus::UNKNOWN ||
             battStatus == BatteryStatus::UNKNOWN) {
        Serial.println("FireStatus: " + String((int) fireStatus) + " BattStatus: " + String((int) battStatus));
        runBluetoothController();
        delay(500);
        messageManager.run();
        delay(500);
        processMessages();
        if (cntr == 5) {
          requestFireStatus();
          requestBattStatus();
          cntr = 0;
        } else {
          cntr++;
        }
      }
      timerMngr.enableAll();
    }

  public:
    /**
      TODO - Add comment here
    */
    void updateBattStatus(BatteryStatus newBattStatus) {
      if (newBattStatus != battStatus) {
        battStatus = newBattStatus;
      }
    }

    /**
      TODO - Add comment here
    */
    void updateFireStatus(FireplaceStatus newFireStatus) {
      if (newFireStatus != fireStatus) {
        fireStatus = newFireStatus;
      }
    }

    /**
      TODO - Add comment here
    */
    void requestFireStatus() {
      messageManager.addOutboundMsg(REQ_FIRE_STATUS_MSG);
    }

    /**
      TODO - Add comment here
    */
    void requestBattStatus() {
      messageManager.addOutboundMsg(REQ_BATT_STATUS_MSG);
    }

    /**
      TODO - Add comment here
    */
    void run() {

      // TODO - explain this
      if (!isInit) {
        while (!btController.isConnected()) {
          runBluetoothController();
          delay(500);
        }
        // Run Initialization
        runInitialize();
        isInit = true;
      }

      // Check to see if the Bluetooth module is connected
      if (btController.isConnected()) {

        // This method runs if the bluetooth connection was restored
        if (fireStatus == FireplaceStatus::UNKNOWN
            || battStatus == BatteryStatus::UNKNOWN) {
          runInitialize();
        }
        if (activityMonitor.isActive()) {

          // This area gets run if the activity monitor shows the room is ACTIVE
          if (thermostat.getMode() == ThermostatMode::OFF) {
            if (fireStatus == FireplaceStatus::RUNNING) {
              messageManager.addOutboundMsg(CMD_FIRE_OFF_MSG);
              messageManager.addOutboundMsg(REQ_FIRE_STATUS_MSG);
            }
            if (fan.isRunning()) {
              fan.stop();
            }
          } else {

            // TODO - explain this
            if (thermostat.getMode() == ThermostatMode::HEATING) {
              if (fireStatus == FireplaceStatus::OFF) {
                messageManager.addOutboundMsg(CMD_FIRE_ON_MSG);
                messageManager.addOutboundMsg(REQ_FIRE_STATUS_MSG);
              }
              if (!fan.isRunning()) {
                fan.start();
              }
            }

            // TODO - explain this
            if (thermostat.getMode() == ThermostatMode::COOLING) {
              if (fireStatus == FireplaceStatus::RUNNING) {
                messageManager.addOutboundMsg(CMD_FIRE_ON_MSG);
                messageManager.addOutboundMsg(REQ_FIRE_STATUS_MSG);
              }
              if (!fan.isRunning()) {
                fan.start();
              }
            }
          }
        } else {
          // This area gets run if the activity monitor shows the room is INACTIVE
          if(fireStatus == FireplaceStatus::RUNNING){
            messageManager.addOutboundMsg(CMD_FIRE_OFF_MSG);
            messageManager.addOutboundMsg(REQ_FIRE_STATUS_MSG);
          }
          if(fan.isRunning()){
            fan.stop();
          }        
        }



        // Request update on battery status
        if (battUpdateCounter < battUpdateCounterMax) {
          battUpdateCounter++;
        } else {
          requestBattStatus();
          battUpdateCounter = 0;
        }

        // Request update on fire status
        if (fireUpdateCounter < fireUpdateCounterMax) {
          fireUpdateCounter++;
        } else {
          requestFireStatus();
          fireUpdateCounter = 0;
        }

      } else {
        
        if (fireStatus != FireplaceStatus::UNKNOWN ||  battStatus != BatteryStatus::UNKNOWN) {
          fireStatus = FireplaceStatus::UNKNOWN;
          battStatus = BatteryStatus::UNKNOWN;
          battUpdateCounter = 0;
          fireUpdateCounter = 0;
          timerMngr.enableTimerByType((int) TimerType::MAIN_APP);
        }
        if(thermostat.getMode() == ThermostatMode::COOLING && !fan.isRunning()){
          fan.start();
        }else if(thermostat.getMode() == ThermostatMode::OFF && fan.isRunning()){
          fan.stop();
        }
        
        
        runBluetoothController();
      }
    }
};

MainApp mainApp;


/**
   Define methods to be run by timers
*/
void processMessages() {
  // Check if a message ready to be processed
  while (messageManager.availableInboundMsg()) {
    // Get the latest inbound message
    String msg = messageManager.getInboundMessage();
    // Create a temporary queue for message
    SimpleQueue tmpQueue;
    // Parse the message into the temporary queue object
    messageManager.parseMessage(msg, tmpQueue);
    // Get the first element in the queue
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    // Handle the message based on its type
    if (msgType == MessageType::HRBT) {
      digitalWrite(heartBtLedPin, HIGH);
      timer.setTimeout(500, runTimeout);
    } else if (msgType == MessageType::INFO) {
      MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();
      int msgVal = (tmpQueue.elementAt(2)).toInt();
      if (msgSpec == MessageSpecifier::BATT) {
        mainApp.updateBattStatus((BatteryStatus) msgVal);
      } else if (msgSpec == MessageSpecifier::FIRE) {
        mainApp.updateFireStatus((FireplaceStatus) msgVal);
      }
    }
  }
}
void runMainApp() {
  mainApp.run();
}
void runThermostat() {
  thermostat.run();
}

void runBluetoothController() {
  btController.run();
}

void runMessageManager() {
  messageManager.run();
}

void runActivityMonitor() {
  activityMonitor.run();
}

void setup() {
  Serial.begin(38400);

  // Set OUTPUT pinModes
  pinMode(heartBtLedPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  // Set INPUT pinModes
  pinMode(btStatePin, INPUT);
  pinMode(heatPin, INPUT);
  pinMode(coolPin, INPUT);
  pinMode(pirPin, INPUT);

  runBluetoothController();

  // Messaging Timers
  timerMngr.addTimer((int)TimerType::PROCESS_MSGS, timer.setInterval(500, processMessages));
  timerMngr.addTimer((int)TimerType::MSG_MANAGER, timer.setInterval(500, runMessageManager));

  // Bluetooth Timer
  timerMngr.addTimer((int)TimerType::BT_CONTROLLER, timer.setInterval(500, runBluetoothController));
  // Main Timer
  timerMngr.addTimer((int)TimerType::MAIN_APP, timer.setInterval(1000, runMainApp));

  // Sensor Timers
  timerMngr.addTimer((int)TimerType::THERMOSTAT, timer.setInterval(1000, runThermostat));
  timerMngr.addTimer((int)TimerType::ACTIVITY_MONITOR, timer.setInterval(1000, runActivityMonitor));
  timerMngr.disableAll();

  // Do pre-initialization runs
  runThermostat();
  runActivityMonitor();
  // Run the Main App
  runMainApp();
}

void loop() {
  timer.run();
}
