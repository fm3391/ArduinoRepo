#include <Fan.h>

#include <ActivityMonitor.h>

#include <Thermostat.h>
#include <BluetoothController.h>

// Libraries
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>

// Constants
const int heartBtLedPin = 12;
const int btStatePin = 6; // Pin that indicate connection status
const int heatPin = 7;
const int coolPin = 8;
const int pirPin = 5;
const int fanPin = 4;

const String SEPERATOR = ":";

const String reqBattStatusMsg = String((int)MessageType::REQ) + SEPERATOR + String((int)MessageSpecifier::BATT);
const String reqFireStatusMsg = String((int)MessageType::REQ) + SEPERATOR + String((int)MessageSpecifier::FIRE);

SimpleTimer timer;
MessageManager messageManager;
BluetoothController btController(btStatePin, -99);
Thermostat thermostat(heatPin, coolPin);
ActivityMonitor activityMonitor(pirPin);
Fan fan(fanPin);

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

    /**
       TODO - Add comment here
    */
    void runStartUpDelay() {
      unsigned long startTime = millis();
      Serial.print("Starting up");
      while ((millis() - startTime) < 5000) {
        // Waiting for all objects to initialize and run
        Serial.print(".");
      }
      Serial.println(".");
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
      messageManager.addOutboundMsg(reqBattStatusMsg);
    }

    /**
      TODO - Add comment here
    */
    void requestBattStatus() {
      messageManager.addOutboundMsg(reqBattStatusMsg);
    }

    /**
      TODO - Add comment here
    */
    void run() {
      if (!isInit) {
        // TODO - Add commment here
        runStartUpDelay();

        // TODO - Add commment here
        if (btController.isConnected()) {
          requestFireStatus();
          requestBattStatus();
        }
        isInit = true;
      }

      // Check to see if the Bluetooth module is connected
      if (btController.isConnected()) {
        // Request update on the fireplace
        requestFireStatus();

        if (battUpdateCounter >= battUpdateCounterMax) {
          requestBattStatus();
          battUpdateCounter = 0;
        } else {
          battUpdateCounter++;
        }
      } else {
        battUpdateCounter = 0;
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


  // Set INPUT pinModes
  pinMode(btStatePin, INPUT);
  pinMode(heatPin, INPUT);
  pinMode(coolPin, INPUT);

  runMainApp();

  timer.setInterval(500, processMessages);
  timer.setInterval(500, runMessageManager);
  timer.setInterval(500, runBluetoothController);
  timer.setInterval(1000, runThermostat);
  timer.setInterval(1000, runActivityMonitor);
  timer.setInterval(1000, runMainApp);

}

void loop() {
  timer.run();
}
