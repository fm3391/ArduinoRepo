#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>
#include <OccupancyMonitor.h>
#include <Thermostat.h>

// Constants
const int fanRelayPin = 0;
const int occupancyInputPin = 0;
const int btStatePin = 0;
const int thermostatHeatPin = 0;
const int thermostatCoolPin = 0;
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
bool isCharging = false;


class Fan {
  private:
    bool isRunning;
    int relayPin;
  public:
    Fan(int pin) {
      this->relayPin = pin;
    }

    void start() {
      if (!isRunning) {
        digitalWrite(relayPin, HIGH);
        isRunning = !isRunning;
      }
    }

    void stop() {
      if (isRunning) {
        digitalWrite(relayPin, LOW);
        isRunning = !isRunning;
      }
    }

};





// Object Instantiations
SimpleTimer timer;
OccupancyMonitor occupancyMonitor;
MessageManager messageManager;
Thermostat thermostat(0, 0);

/*
  Main Class for managing the FireplaceController and Fan
*/
class RoomManager {
  private:
    bool isInit = false;
    OccupancyMonitor *occupancyMonitor;
    Thermostat *thermostat;
    MessageManager *messageManager;
    Fan fan;

    FireplaceStatus fireplaceStatus = FireplaceStatus::UNKNOWN;
    BatteryStatus batteryStatus = BatteryStatus::UNKNOWN;

  public:
    RoomManager(Thermostat &thermostatIn, MessageManager &messageManagerIn,
                OccupancyMonitor &occupancyMonitor, const int &fanRelayPinIn): fan(fanRelayPinIn) {
      this->messageManager = &messageManagerIn;
      this->thermostat = &thermostatIn;
      this->occupancyMonitor = &occupancyMonitor;
    }

    void handleBatteryStatusUpdate(BatteryStatus newStatus) {
      if(newStatus != batteryStatus){
        batteryStatus = newStatus;
      }
    }

    void handleFireplaceStatusUpdate(FireplaceStatus newStatus) {
      if(newStatus != fireplaceStatus){
        fireplaceStatus = newStatus;
      }
    }
    void run() {
      if (!isInit) {
        thermostat->run();
        messageManager->run();
        occupancyMonitor->run();
        if (isConnected) {
          messageManager->addOutboundMsg(reqBatteryMsg);
          messageManager->addOutboundMsg(reqFireMsg);
        }
        isInit = true;
      }

      if(occupancyMonitor->roomOccupied){
        
      }else{
        
      }
      
    }

};

// RoomManager Instantiation
RoomManager roomManager(thermostat, messageManager, occupancyMonitor, fanRelayPin);

/*
 * Processes messages from the FireplaceController
 */
void processInboundMsgs() {
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);

    MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();
    if(msgSpec == MessageSpecifier::BATT){
      roomManager.handleBatteryStatusUpdate((BatteryStatus) (tmpQueue.elementAt(2)).toInt());
    }else if(msgSpec == MessageSpecifier::FIRE){
      roomManager.handleFireplaceStatusUpdate((FireplaceStatus) (tmpQueue.elementAt(2)).toInt());
    }
  }
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

void checkBluetoothConnection() {
  if (digitalRead(btStatePin) == HIGH && !isConnected) {
    isConnected = true;
  } else if (digitalRead(btStatePin) == LOW && isConnected) {
    isConnected = false;
  }
}


void setup() {
  Serial.begin(9600);
  checkBluetoothConnection();
  attachInterrupt(digitalPinToInterrupt(occupancyInputPin), activityDetected, RISING);
  timer.setInterval(1000, runOccupancyMonitor);
  timer.setInterval(1000, runMessageManager);
  timer.setInterval(1000, runThermostat);
  timer.setInterval(1000, checkBluetoothConnection);
  timer.setInterval(5000, runRequestInfo);
}

void loop() {
  timer.run();
}
