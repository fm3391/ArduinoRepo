/* Included Libraries */
#include <ValveControllerState.h>
#include <MessageManager.h>
#include <SimpleQueue.h>
#include <ChargeController.h>
#include <ValveController.h>
#include <Mosfet.h>
#include <MD10C.h>
#include <SimpleTimer.h>
#include <BatteryStatus.h>

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

const int overrideOnPin = 10;
const int overrideOffPin = 11;


enum class FireplaceStatus {
  OFF,
  RUNNING
};
enum class SystemMode {
  LOW_BATT,
  NORMAL,
  OVERRIDE
};

class FireplaceController {
  private:
    SystemMode mode;
    FireplaceStatus fireplaceStatus;
    MessageManager *messageManager;
    ChargeController *chargeController;
    ValveController valveController;

    void init() {
      chargeController->run();
      mode = SystemMode::NORMAL;
      messageManager->run();
      fireplaceStatus = FireplaceStatus::OFF;
      valveController.closeValve(chargeController->getVoltage()); 
    }


    void handleRequestMessage(SimpleQueue &queueIn){
      String reqType = queueIn.elementAt(1);
      if(reqType == "BATT"){
        
      }else if(reqType == "FIRE"){
        
      }
    }

    void processMessages() {
      while(messageManager->availableInboundMsg()){
        SimpleQueue tmpQueue;
        String msg = messageManager->getInboundMessage();
        messageManager->parseMessage(msg, tmpQueue);
        String msgIdentifier = tmpQueue.elementAt(0);
        if(msgIdentifier == "REQ"){
          handleRequestMessage(tmpQueue);
        }else if(msgIdentifier == "CMD"){
          if(mode != SystemMode::OVERRIDE){
            
          }
        }
      }
    }

    void handleOverrideMode() {
      if (digitalRead(overrideOnPin) == HIGH && fireplaceStatus == FireplaceStatus::OFF) {
        valveController.openValve(chargeController->getVoltage());
        fireplaceStatus = FireplaceStatus::RUNNING;
      } else if (digitalRead(overrideOffPin) == HIGH && fireplaceStatus == FireplaceStatus::RUNNING) {
        valveController.closeValve(chargeController->getVoltage());
        fireplaceStatus = FireplaceStatus::OFF;
      } else {
        mode = SystemMode::NORMAL;
      }
      // Process inbound messages
      processMessages();
    }

    void handleNormalMode() {

    }

    void handleLowBattMode() {

    }

  public:
    FireplaceController(MessageManager &messageManagerIn, ChargeController &chargeControllerIn)
      : valveController(vcDirPin, vcPwmPin, vcMosfetPin) {
      this->messageManager = &messageManagerIn;
      this->chargeController = &chargeControllerIn;
      init();
    }

    void run() {
      switch (mode) {
        case SystemMode::OVERRIDE:
          handleOverrideMode();
          break;
        case SystemMode::NORMAL:
          handleNormalMode();
          break;
        case SystemMode::LOW_BATT:
          handleLowBattMode();
          break;
      }
    }
};

// Object Instantiations
SimpleTimer timer;
MessageManager messageManager;
ChargeController chargeController(ccRelayPin, ccBattInputPin);
FireplaceController fireplaceController(messageManager, chargeController);

void runFireplaceController() {
  fireplaceController.run();
}

void runMessageManager() {
  messageManager.run();
}

void runChargeController() {
  chargeController.run();
}

void setup() {
  Serial.begin(38400);
  pinMode(ccRelayPin, OUTPUT);
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);
  digitalWrite(ccRelayPin, LOW);
  digitalWrite(vcMosfetPin, LOW);

  timer.setInterval(500, runMessageManager);
  timer.setInterval(1000, runFireplaceController);
  timer.setInterval(30000, runChargeController);
}

void loop() {
  timer.run();
}
