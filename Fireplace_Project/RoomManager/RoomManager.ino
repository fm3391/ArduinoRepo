#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <OccupancyMonitor.h>
#include <Thermostat.h>

// Constants
const int heartBtLedPin = 12;
const int fanRelayPin = 11;
const int occupancyInputPin = 2;
const int btStatePin = 10;
const int thermostatHeatPin = 9;
const int thermostatCoolPin = 8;
const String MSG_SEPERATOR = ":";

// Message used to request the current battery status from the FireplaceController
const String reqBatteryMsg =  String((int) MessageType::REQ) + MSG_SEPERATOR +
                              String((int) MessageSpecifier::BATT);
//
const String reqFireMsg =  String((int) MessageType::REQ) + MSG_SEPERATOR +
                           String((int) MessageSpecifier::FIRE);
//
const String cmdFireOnMsg =  String((int) MessageType::CMD) + MSG_SEPERATOR +
                             String((int) MessageSpecifier::FIRE) + MSG_SEPERATOR +
                             String((int) MessageCmd::FIRE_ON);
//
const String cmdFireOffMsg =  String((int) MessageType::CMD) + MSG_SEPERATOR +
                              String((int) MessageSpecifier::FIRE) + MSG_SEPERATOR +
                              String((int) MessageCmd::FIRE_OFF);

// Bluetooth Connection State variable
bool isConnected = false;

// Object Instantiations
SimpleTimer timer;
SimpleTimer timeOutTimer;
OccupancyMonitor occupancyMonitor;
MessageManager messageManager;
Thermostat thermostat(thermostatHeatPin, thermostatCoolPin);

/*
  Main Class for managing the FireplaceController and Fan
*/
class RoomManager {
  private:
    bool isInit = false;


  public:
    RoomManager() {

    }

    void run() {
      if (!isInit) {

        isInit = true;
      }


      
    }
};

// RoomManager Instantiation






/*
   Processes messages from the FireplaceController
*/
void processInboundMsgs() {
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    if (msgType == MessageType::HRBT) {
      digitalWrite(heartBtLedPin, HIGH);
      timeOutTimer.setTimeout(500, runTimeout);
    } else {
      MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();
      if (msgSpec == MessageSpecifier::BATT) {
        //TODO
      } else if (msgSpec == MessageSpecifier::FIRE) {
        //TODO
      }
    }

  }
}

void runTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

void activityDetected() {
  occupancyMonitor.activityDetected();
}

void runOccupancyMonitor() {
  occupancyMonitor.run();
}

void runMessageManager() {
  messageManager.run();
}

void runThermostat() {
  thermostat.run();
}

void runRequestInfo() {
  if (isConnected) {
    messageManager.addOutboundMsg(reqBatteryMsg);
    messageManager.addOutboundMsg(reqFireMsg);
  }
}

void updateBluetoothConnection() {
  if (digitalRead(btStatePin) == HIGH && !isConnected) {
    isConnected = true;
  } else if (digitalRead(btStatePin) == LOW && isConnected) {
    isConnected = false;
  }
}

void setup() {
  Serial.begin(38400);
  pinMode(heartBtLedPin, OUTPUT);
  updateBluetoothConnection();
  digitalWrite(heartBtLedPin, LOW);
  attachInterrupt(digitalPinToInterrupt(occupancyInputPin), activityDetected, RISING);
  timer.setInterval(500, runOccupancyMonitor);
  timer.setInterval(250, runMessageManager);
  timer.setInterval(1000, runThermostat);
  timer.setInterval(500, updateBluetoothConnection);
  timer.setInterval(5000, runRequestInfo);
}

void loop() {
  timer.run();
  timeOutTimer.run();
}
