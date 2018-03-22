#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <OccupancyMonitor.h>
#include <Thermostat.h>

// Constants
const int heartBtLedPin = 13;
const int fanRelayPin = 11;
const int occupancyInputPin = 2;
const int btStatePin = 10;
const int thermostatHeatPin = 9;
const int thermostatCoolPin = 8;

const int batteryIndicatorPin = 5;
const int statusIndicatorPin = 6;

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


class Fan {
  private:
    int relayPin;
    bool isRunning;
  public:
    Fan(int relayPin) {
      this->relayPin = relayPin;
    }

    bool fanIsRunning() {
      return this->isRunning;
    }

    void start() {
      digitalWrite(this->relayPin, HIGH);
      isRunning = true;
    }

    void stop() {
      digitalWrite(this->relayPin, LOW);
      isRunning = false;
    }
};



// Object Instantiations
SimpleTimer mainTimer;
SimpleTimer secondaryTimer;
SimpleTimer timeOutTimer;
OccupancyMonitor occupancyMonitor;
MessageManager messageManager;
Thermostat thermostat(thermostatHeatPin, thermostatCoolPin);



Fan fan(fanRelayPin);

/*
  Main Class for managing the FireplaceController and Fan
*/
class Main {
  private:
    bool isInit = false;
    FireplaceStatus fireplaceStatus = FireplaceStatus::UNKNOWN;
    BatteryStatus batteryStatus = BatteryStatus::UNKNOWN;

  public:
    Main() {

    }

    void run() {
      if (!isInit) {
        if (isConnected) {
          messageManager.addOutboundMsg(reqBatteryMsg);
          messageManager.addOutboundMsg(reqFireMsg);
        }
        fan.stop();
        isInit = true;
      }

      // Begin running rules
      if (!isConnected && fireplaceStatus != FireplaceStatus::UNKNOWN
          && batteryStatus != BatteryStatus::UNKNOWN) {
        fireplaceStatus = FireplaceStatus::UNKNOWN;
        batteryStatus = BatteryStatus::UNKNOWN;
      }

      if (isConnected) {
        if (thermostat.getMode() == ThermostatMode::OFF) {
          // Make sure the fan is OFF and the Fireplace is OFF
          if (fan.fanIsRunning()) {
            fan.stop();
          }

          if (fireplaceStatus == FireplaceStatus::RUNNING) {
            messageManager.addOutboundMsg(cmdFireOffMsg);
          }
        } else {

        }

      } else {

      }
    }
};

// RoomManager Instantiation
Main main;


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

void runMain() {
  main.run();
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
  
  // Set OUTPUT pinModes
  pinMode(heartBtLedPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(batteryIndicatorPin, OUTPUT);
  pinMode(statusIndicatorPin, OUTPUT);
  
  // Set INPUT pinModes
  pinMode(btStatePin, INPUT);
  pinMode(thermostatHeatPin, INPUT);
  pinMode(thermostatCoolPin, INPUT);
  
  // Initialize pins
  digitalWrite(heartBtLedPin, LOW);

  // Check bluetooth connection
  updateBluetoothConnection();

  // Begin timer and interrupt setup
  //mainTimer.setInterval(1000, runMain);
  //secondaryTimer.setInterval(500, runOccupancyMonitor);
  secondaryTimer.setInterval(250, processInboundMsgs);
  secondaryTimer.setInterval(500, runMessageManager);
  //secondaryTimer.setInterval(1000, runThermostat);
  secondaryTimer.setInterval(500, updateBluetoothConnection);
  //secondaryTimer.setInterval(2000, runRequestInfo);
}

void loop() {
  mainTimer.run();
  secondaryTimer.run();
  timeOutTimer.run();
}
