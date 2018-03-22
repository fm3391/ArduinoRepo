// Libraries
#include <Fan.h>
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

// TODO
FireplaceStatus fireplaceStatus = FireplaceStatus::UNKNOWN;
BatteryStatus batteryStatus = BatteryStatus::UNKNOWN;
bool isInit = false;

// Object Instantiations
SimpleTimer mainTimer;
SimpleTimer hrtBtTimer;
OccupancyMonitor occupancyMonitor;
MessageManager messageManager;
Thermostat thermostat(thermostatHeatPin, thermostatCoolPin);
Fan fan(fanRelayPin);

/*
  Main Class for managing the FireplaceController and Fan
*/
class Main {
  public:
    void run() {
      if (!isInit) {

        isInit = true;
      }
    

    }
};

// RoomManager Instantiation
Main main;


void runHbTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

/*
   Processes messages from the FireplaceController
*/
void processInboundMsgs() {
  Serial.println("Processing Messages");
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    if (msgType == MessageType::HRBT) {
      digitalWrite(heartBtLedPin, HIGH);
      hrtBtTimer.setTimeout(500, runHbTimeout);
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
  Serial.println("Main Running");
  main.run();
}

void activityDetected() {
  occupancyMonitor.activityDetected();
}
void runMessageManager() {
  Serial.println("Message Manager Running");
  messageManager.run();
}

void updateBluetoothConnection() {
  Serial.println("Bluetooth");
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
  mainTimer.setInterval(1000, runMain);
  mainTimer.setInterval(250, processInboundMsgs);
  mainTimer.setInterval(500, runMessageManager);
  mainTimer.setInterval(500, updateBluetoothConnection);
}

void loop() {
  mainTimer.run();
  hrtBtTimer.run();
}
