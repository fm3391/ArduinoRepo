// Libraries
#include <Fan.h>
#include <ActivityMonitor.h>
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <Thermostat.h>


// Constants
const int heartBtLedPin = 12;
const int fanRelayPin = 11;
const int btStatePin = 10;
const int thermostatHeatPin = 8;
const int thermostatCoolPin = 7;
const int battStatusRedPin = 6;     // PWM
const int battStatusGreenPin = 5;   // PWM
const int pirInputPin = 3;

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

// TODO
FireplaceStatus fireplaceStatus = FireplaceStatus::UNKNOWN;
BatteryStatus batteryStatus = BatteryStatus::UNKNOWN;
bool isInit = false;

SimpleTimer timer1;
SimpleTimer timer2;
MessageManager messageManager;
Thermostat thermostat(thermostatHeatPin, thermostatCoolPin);
ActivityMonitor activityMonitor;
Fan fan(fanRelayPin);



//
void runConnectedRoutine() {

}

//
void runDisConnectedRoutine() {

}

/*

*/
int runMain() {
  if (!isInit) {
    if (isConnected) {
      messageManager.addOutboundMsg(reqBatteryMsg);
      messageManager.addOutboundMsg(reqFireMsg);
    }
    fan.stop();
  }

  if (isConnected) {
    ThermostatMode thermoMode = thermostat.getMode();
    if (thermoMode == ThermostatMode::OFF) {
      if (fireplaceStatus == FireplaceStatus::RUNNING) {
        messageManager.addOutboundMsg(cmdFireOffMsg);
      }
    } else if (thermoMode == ThermostatMode::HEATING) {
      if (fireplaceStatus == FireplaceStatus::OFF) {
        messageManager.addOutboundMsg(cmdFireOnMsg);
      }

    } else if (thermoMode == ThermostatMode::COOLING) {
      if (fireplaceStatus == FireplaceStatus::RUNNING) {
        messageManager.addOutboundMsg(cmdFireOffMsg);
      }
    }
  }



  return 0;
}



void runTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

void processMessages() {
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    if (msgType == MessageType::HRBT) {
      digitalWrite(heartBtLedPin, HIGH);
      timer2.setTimeout(500, runTimeout);
    } else if (msgType == MessageType::INFO) {
      MessageSpecifier msgSpec = (MessageSpecifier) (tmpQueue.elementAt(1)).toInt();
      int msgVal = (tmpQueue.elementAt(2)).toInt();

      if (msgSpec == MessageSpecifier::BATT) {
        BatteryStatus newBattStatus = (BatteryStatus) msgVal;
        if (newBattStatus != batteryStatus) {
          batteryStatus = newBattStatus;
        }

      } else if (msgSpec == MessageSpecifier::FIRE) {
        FireplaceStatus newFireStatus = (FireplaceStatus) msgVal;
        if (fireplaceStatus != newFireStatus) {
          fireplaceStatus = newFireStatus;
        }
      }
    }
  }
}

void runMessageManager() {
  messageManager.run();
}

void runThermostat() {
  thermostat.run();
}

void runInfoRequest() {
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

void runActivityMonitor() {
  activityMonitor.run();
}

void setup() {
  Serial.begin(38400);

  // Set OUTPUT pinModes
  pinMode(heartBtLedPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(battStatusRedPin, OUTPUT);
  pinMode(battStatusGreenPin, OUTPUT);

  // Set INPUT pinModes
  pinMode(btStatePin, INPUT);
  pinMode(thermostatHeatPin, INPUT);
  pinMode(thermostatCoolPin, INPUT);
  pinMode(pirInputPin, INPUT);

  // Check bluetooth connection
  updateBluetoothConnection();

  // Timer 1 Functions
  timer1.setInterval(500, runMessageManager);
  timer1.setInterval(500, processMessages);
  timer1.setInterval(1000, runMain);
  timer1.setInterval(2000, runInfoRequest);

  // Timer 2 Functions
  timer2.setInterval(1000, updateBluetoothConnection);
  timer2.setInterval(1000, runActivityMonitor);
  timer2.setInterval(1000, runThermostat);
}

void loop() {
  timer1.run();
  timer2.run();
}
