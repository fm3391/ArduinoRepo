/* Included Libraries */
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleQueue.h>
#include <ChargeController.h>
#include <ValveController.h>
#include <Mosfet.h>
#include <MD10C.h>
#include <SimpleTimer.h>
#include <BluetoothController.h>
/*
  Description:


*/

// Pin Declarations
const int vcDirPin = 7; // ValveController DIR pin
const int vcPwmPin = 9; // ValveController PWM pin
const int vcMosfetPin = 8; // ValveController Mosfet pin

const int ccRelayPin = 12; // ChargeController Relay pin
const int ccBattInputPin = A0; // ChargeController Input pin

const int btMosfetPin = 5; // Mosfet that controls HC-05 module
const int btStatePin = 6; // Pin that indicate connection status

const int overrideOnPin = 11; // TODO
const int overrideOffPin = 10; // TODO

const String EMPTY = "";
const String SEPERATOR = ":";

bool lowBatt = false;

SimpleTimer timer;
SimpleTimer tmpTimer;

ChargeController chargeController(ccRelayPin, ccBattInputPin);
BluetoothController btController(btStatePin, btMosfetPin);
MessageManager messageManager;


class MainApp {
  private:
    SystemMode mode = SystemMode::NORMAL;
    ValveController valveController;
    FireplaceStatus fireplaceStatus;
    bool isInit = false;

    void setFireplaceState(FireplaceStatus newFireplaceStatus) {
      switch (newFireplaceStatus) {
        case FireplaceStatus::RUNNING:
          Serial.println("FIRE ON");
          valveController.openValve(chargeController.getVoltage());
          fireplaceStatus = FireplaceStatus::RUNNING;
          break;
        case FireplaceStatus::OFF:
          Serial.println("FIRE OFF");
          valveController.closeValve(chargeController.getVoltage());
          fireplaceStatus = FireplaceStatus::OFF;
          break;
      }
    }

  public:
    MainApp(): valveController(vcDirPin, vcPwmPin, vcMosfetPin) {

    }

    void handleCmdMessage(MessageCmd msgCmd) {
      if (mode == SystemMode::NORMAL && btController.isConnected()) {
        if (msgCmd == MessageCmd::FIRE_ON &&
            fireplaceStatus == FireplaceStatus::OFF) {
          setFireplaceState(FireplaceStatus::RUNNING);
        } else if (msgCmd == MessageCmd::FIRE_OFF &&
                   fireplaceStatus == FireplaceStatus::RUNNING) {
          setFireplaceState(FireplaceStatus::OFF);
        }
      }
    }

    FireplaceStatus getFireStatus() {
      return fireplaceStatus;
    }

    void run() {
      if (!isInit) {
        setFireplaceState(FireplaceStatus::OFF);
        isInit = true;
      }

      if (chargeController.getBatteryStatus() > BatteryStatus::WARNING &&
          !btController.isEnabled()) {
        btController.setEnabled(true);
      } else if (chargeController.getBatteryStatus() <= BatteryStatus::WARNING &&
                 btController.isEnabled()) {
        btController.setEnabled(false);
      }

      if (mode == SystemMode::NORMAL &&
          (digitalRead(overrideOnPin) == HIGH ||
           digitalRead(overrideOffPin) == HIGH)) {
        mode = SystemMode::OVERRIDE;
      } else if (mode == SystemMode::OVERRIDE &&
                 (digitalRead(overrideOnPin) == LOW &&
                  digitalRead(overrideOffPin) == LOW)) {
        mode = SystemMode::NORMAL;
      }

      if (mode == SystemMode::OVERRIDE) {
        if (digitalRead(overrideOnPin) == HIGH &&
            fireplaceStatus == FireplaceStatus::OFF) {
          setFireplaceState(FireplaceStatus::RUNNING);
        } else if (digitalRead(overrideOffPin) == HIGH &&
                   fireplaceStatus == FireplaceStatus::RUNNING) {
          setFireplaceState(FireplaceStatus::OFF);
        }
      }

      if (mode == SystemMode::NORMAL) {
        if (!btController.isConnected() &&
            fireplaceStatus == FireplaceStatus::RUNNING) {
          setFireplaceState(FireplaceStatus::OFF);
        }
      }


    }

};

MainApp mainApp;



void processMessages() {
  if (btController.isConnected()) {
    while (messageManager.availableInboundMsg()) {
      String msg = messageManager.getInboundMessage();
      SimpleQueue tmpQueue;
      messageManager.parseMessage(msg, tmpQueue);
      MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
      MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();
      if (msgType == MessageType::CMD) {

        mainApp.handleCmdMessage((MessageCmd)(tmpQueue.elementAt(2)).toInt());
      } else if (msgType == MessageType::REQ) {
        if (msgSpec == MessageSpecifier::FIRE) {
          messageManager.addOutboundMsg(String((int)MessageType::INFO) + SEPERATOR +
                                        String((int) MessageSpecifier::FIRE) + SEPERATOR +
                                        String((int) mainApp.getFireStatus()));
        } else if (msgSpec == MessageSpecifier::BATT) {
          messageManager.addOutboundMsg(String((int)MessageType::INFO) + SEPERATOR +
                                        String((int) MessageSpecifier::BATT) + SEPERATOR +
                                        String((int) chargeController.getBatteryStatus()));
        }
      }

    }

  } else if (messageManager.availableInboundMsg()) {
    messageManager.clearInboundMsgs();
  }
}

void runMainApp() {
  mainApp.run();
}

void runChargeController() {
  chargeController.run();
}

void runBluetoothController() {
  btController.run();
}

void runMessageManager() {
  messageManager.run();
}

void sendHeartBtMsg() {
  if(btController.isConnected()){
    messageManager.addOutboundMsg(String((int)MessageType::HRBT));
  }
}

void setup() {
  Serial.begin(38400);

  // Output pins
  pinMode(ccRelayPin, OUTPUT);
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);
  pinMode(btMosfetPin, OUTPUT);

  // Input pins
  pinMode(overrideOnPin, INPUT);
  pinMode(overrideOffPin, INPUT);
  pinMode(btStatePin, INPUT);

  // Setting initial pin setting
  digitalWrite(ccRelayPin, LOW);
  digitalWrite(vcMosfetPin, LOW);

  runChargeController();

  // Setup Timers
  timer.setInterval(30000, runChargeController);
  timer.setInterval(500, runMessageManager);
  timer.setInterval(500, processMessages);
  timer.setInterval(1000, runBluetoothController);
  timer.setInterval(2000, sendHeartBtMsg);
  timer.setInterval(1000, runMainApp);
}

void loop() {
  timer.run();
}
