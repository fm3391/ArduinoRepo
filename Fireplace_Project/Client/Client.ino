// Libraries
#include <Fan.h>
#include <ActivityMonitor.h>
#include <Thermostat.h>
#include <BluetoothController.h>
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>

// Input pins
const int btStatePin = 6; // Pin that indicate connection status
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

int mainAppTimerId;

void runTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

class MainApp {
  private:
    bool isInit = false;
    FireplaceStatus fireStatus = FireplaceStatus::UNKNOWN;
    BatteryStatus battStatus = BatteryStatus::UNKNOWN;
    int battUpdateCounter = 0;
    const int battUpdateCounterMax = 60;
    int fireUpdateCounter = 0;
    const int fireUpdateCounterMax = 5;

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
      if (!isInit) {
        Serial.print("Waiting for connection");
        while (!btController.isConnected()) {
          Serial.print(".");
          delay(500);
        }
        Serial.println("");
        Serial.println("Connected! Beginning initialize");
        requestFireStatus();
        requestBattStatus();
        int waitCounter = 0;
        while (fireStatus == FireplaceStatus::UNKNOWN || battStatus == BatteryStatus::UNKNOWN) {
          Serial.println("FireStatus: " + String((int) fireStatus) + " BattStatus: " + String((int) battStatus));
          if (waitCounter == 5) {
            requestFireStatus();
            requestBattStatus();
            waitCounter = 0;
          } else {
            waitCounter++;
          }
          delay(500);
        }
        Serial.println("Finished!");
        timer.restartTimer(mainAppTimerId);
        timer.enable(mainAppTimerId);

        isInit = true;
      }

      // Check to see if the Bluetooth module is connected
      if (btController.isConnected()) {







        // Request update on battery status
        if (battUpdateCounter < battUpdateCounterMax) {
          battUpdateCounter++;
        } else {
          requestBattStatus();
          battUpdateCounter = 0;
        }


        if (fireUpdateCounter < fireUpdateCounterMax) {
          fireUpdateCounter++;
        } else {
          requestFireStatus();
          fireUpdateCounter = 0;
        }


      } else {
        battUpdateCounter = 0;
        fireUpdateCounter = 0;
      }


    }
};

MainApp mainApp;

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
  timer.setInterval(500, processMessages);
  timer.setInterval(500, runMessageManager);

  // Bluetooth Timer
  timer.setInterval(500, runBluetoothController);

  // Main Timer
  mainAppTimerId = timer.setInterval(1000, runMainApp);
  timer.disable(mainAppTimerId);
  runMainApp();

  // Sensor Timers
  timer.setInterval(1000, runThermostat);
  timer.setInterval(1000, runActivityMonitor);
}

void loop() {
  timer.run();
}
