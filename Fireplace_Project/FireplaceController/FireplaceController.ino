/* Included Libraries */
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleQueue.h>
#include <ChargeController.h>
#include <ValveController.h>
#include <Mosfet.h>
#include <MD10C.h>
#include <SimpleTimer.h>

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

const long timeOutMax = (long) 600000;

BluetoothState btState;
bool isConnected = false;
bool lowBatt = false;

SimpleTimer timer;
int timeOutTimerId;


/*

*/
void setBluetoothState(BluetoothState newState) {
  if (newState != btState) {
    switch (newState) {
      case BluetoothState::ENABLED:
        digitalWrite(btMosfetPin, HIGH);
        btState = BluetoothState::ENABLED;
        break;
      case BluetoothState::DISABLED:
        digitalWrite(btMosfetPin, LOW);
        btState = BluetoothState::DISABLED;
        break;
    }
  }
}

// Main Class
class FireplaceController {
  private:
    SystemMode mode = SystemMode::NORMAL;
    MessageManager *messageManager;
    ChargeController *chargeController;
    ValveController valveController;
    FireplaceStatus fireplaceStatus;
    bool isInit = false;


    /*

    */
    void setFireplaceState(FireplaceStatus newFireplaceStatus) {
      if (fireplaceStatus != newFireplaceStatus) {
        switch (newFireplaceStatus) {
          case FireplaceStatus::RUNNING:
            valveController.openValve(chargeController->getVoltage());
            fireplaceStatus = FireplaceStatus::RUNNING;
            timeOutTimerId = timer.setTimeout(timeOutMax, runTimeout);
            break;
          case FireplaceStatus::OFF:
            valveController.closeValve(chargeController->getVoltage());
            fireplaceStatus = FireplaceStatus::OFF;
            timer.deleteTimer(timeOutTimerId);
            break;
        }
        handleReqMsg(MessageSpecifier::FIRE);
      }
    }


  public:
    FireplaceController(MessageManager & messageManagerIn, ChargeController & chargeControllerIn)
      : valveController(vcDirPin, vcPwmPin, vcMosfetPin) {
      this->messageManager = &messageManagerIn;
      this->chargeController = &chargeControllerIn;
    }

    SystemMode getMode() {
      return mode;
    }

    FireplaceStatus getFireplaceStatus() {
      return fireplaceStatus;
    }

    /*

    */
    void handleCmdMsg(MessageSpecifier msgSpec, MessageCmd msgCmd) {
      if (mode != SystemMode::OVERRIDE) {
        if (msgCmd == MessageCmd::FIRE_ON) {
          if (fireplaceStatus != FireplaceStatus::RUNNING && !lowBatt) {
            setFireplaceState(FireplaceStatus::RUNNING);
          }
        } else if (msgCmd == MessageCmd::FIRE_OFF) {
          if (fireplaceStatus == FireplaceStatus::RUNNING) {
            setFireplaceState(FireplaceStatus::OFF);
          }
        }

      }
    }

    /*

    */
    void handleReqMsg(MessageSpecifier msgSpec) {
      String msgOut = "";
      switch (msgSpec) {
        case MessageSpecifier::BATT:
          msgOut = String((int)MessageType::INFO) + SEPERATOR + String((int) MessageSpecifier::BATT)
                   + SEPERATOR + String((int) chargeController->getBatteryStatus());
          messageManager->addOutboundMsg(msgOut);
          break;
        case MessageSpecifier::FIRE:
          msgOut = String((int)MessageType::INFO) + SEPERATOR + String((int) MessageSpecifier::FIRE)
                   + SEPERATOR + String((int) fireplaceStatus);
          messageManager->addOutboundMsg(msgOut);
          break;
      }
    }

    /*

    */
    void run() {
      if (!isInit) {
        chargeController->run();
        messageManager->run();
        setFireplaceState(FireplaceStatus::OFF);
        //if (isConnected) {
        if (true) {
          handleReqMsg(MessageSpecifier::BATT);
          delay(500);
          handleReqMsg(MessageSpecifier::FIRE);
        }
        isInit = true;
      }

      /*

      */
      if (chargeController->getBatteryStatus() == BatteryStatus::LOW_BATT && btState == BluetoothState::ENABLED) {
        setBluetoothState(BluetoothState::DISABLED);
      } else if (chargeController->getBatteryStatus() > BatteryStatus::LOW_BATT && btState == BluetoothState::DISABLED) {
        setBluetoothState(BluetoothState::ENABLED);
      }

      /*

      */

      if (mode == SystemMode::NORMAL) {
        if ((digitalRead(overrideOnPin) == HIGH || digitalRead(overrideOffPin) == HIGH)) {
          mode = SystemMode::OVERRIDE;
        } else if (fireplaceStatus == FireplaceStatus::RUNNING && !isConnected) {
          setFireplaceState(FireplaceStatus::OFF);
        }
      }


      /*

      */
      if (mode == SystemMode::OVERRIDE) {
        if (digitalRead(overrideOnPin) == HIGH && fireplaceStatus != FireplaceStatus::RUNNING) {
          setFireplaceState(FireplaceStatus::RUNNING);
        } else if (digitalRead(overrideOffPin) == HIGH && fireplaceStatus != FireplaceStatus::OFF) {
          setFireplaceState(FireplaceStatus::OFF);
        } else {
          mode = SystemMode::NORMAL;
        }
      }
    }
};

// Object Instantiations
MessageManager messageManager;
ChargeController chargeController(ccRelayPin, ccBattInputPin);
FireplaceController fireplaceController(messageManager, chargeController);


void runTimeout() {
  if (fireplaceController.getFireplaceStatus() == FireplaceStatus::RUNNING && fireplaceController.getMode() == SystemMode::NORMAL) {
    fireplaceController.handleCmdMsg(MessageSpecifier::FIRE, MessageCmd::FIRE_OFF);
  }
}

/*

*/
void processMessages() {
  if (isConnected) {
    while (messageManager.availableInboundMsg()) {
      String msg = messageManager.getInboundMessage();
      SimpleQueue tmpQueue;
      messageManager.parseMessage(msg, tmpQueue);
      MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
      MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();

      switch (msgType) {
        case MessageType::REQ:
          if (msgSpec == MessageSpecifier::BATT || msgSpec == MessageSpecifier::FIRE) {
            fireplaceController.handleReqMsg(msgSpec);
          }
          break;
        case MessageType::CMD:
          if (msgSpec == MessageSpecifier::FIRE) {
            MessageCmd cmd = (MessageCmd) (tmpQueue.elementAt(2)).toInt();
            fireplaceController.handleCmdMsg(msgSpec, cmd);
          }
          break;
      }

    }
  } else if (messageManager.availableInboundMsg()) {
    messageManager.clearInboundMsgs();
  }
}

/*

*/
void runFireplaceController() {
  fireplaceController.run();
}
/*

*/
void runMessageManager() {
  messageManager.run();
}
/*

*/
void runChargeController() {
  chargeController.run();
}
/*

*/
void runConnectionUpdate() {
  int connectionVal = digitalRead(btStatePin);
  if (isConnected && connectionVal == LOW) {
    isConnected = false;
  } else if (!isConnected && connectionVal == HIGH) {
    isConnected = true;
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

  // Update Connection Status
  setBluetoothState(BluetoothState::ENABLED );
  runConnectionUpdate();


  // Setup Timers
  timer.setInterval(500, runMessageManager);
  timer.setInterval(500, runConnectionUpdate);
  timer.setInterval(750, processMessages);
  timer.setInterval(1000, runFireplaceController);
  timer.setInterval(30000, runChargeController);
}

void loop() {
  timer.run();
}
