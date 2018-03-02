/* Included Libraries */
#include <ValveControllerState.h>
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

const int overrideOnPin = 10; // TODO
const int overrideOffPin = 11; // TODO



enum class SystemMode{
  LOW_BATT,OVERRIDE,NORMAL
};

bool overrideOnSelected(){
  boolean isSelected = false;
  if(digitalRead(overrideOnPin) == HIGH){
    isSelected = true;
  }
  return isSelected;
}

bool overrideOffSelected(){
  boolean isSelected = false;
  if(digitalRead(overrideOffPin) == HIGH){
    isSelected = true;
  }
  return isSelected;
}

class FireplaceController {
  private:
    SystemMode mode = SystemMode::NORMAL;
    MessageManager *messageManager;
    ChargeController *chargeController;
    ValveController valveController;
    bool isInit = false;
    int fireplaceState = 0;

    void processMessages(){
      while(messageManager->availableInboundMsg()){
        String msg = messageManager->getInboundMessage();
        SimpleQueue tmpQueue;
        messageManager->parseMessage(msg, tmpQueue);
        if(tmpQueue.elementAt(0) == "REQ"){
          String infoMsg = "";
          if(tmpQueue.elementAt(1) == "BATT"){
            infoMsg = "INFO:BATT:"+String(chargeController->getBatteryLevel());
          }else if(tmpQueue.elementAt(1) == "FIRE"){
            infoMsg = "INFO:FIRE:"+String(fireplaceState);
          }
          if(infoMsg != ""){
            messageManager->addOutboundMsg(infoMsg);
          }
        }else if(tmpQueue.elementAt(0) == "CMD"){
          

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
      
    }

    void run() {
      if(!isInit){
        if(chargeController->getBatteryLevel() == 0){
          mode = SystemMode::LOW_BATT;
        }
        isInit = true;
      }
      processMessages();
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
  Serial.begin(9600);
  pinMode(ccRelayPin, OUTPUT);
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);
  digitalWrite(ccRelayPin, LOW);
  digitalWrite(vcMosfetPin, LOW);

  timer.setInterval(500, runMessageManager);
  timer.setInterval(1000, runFireplaceController);
  timer.setInterval(5000, runChargeController);
}

void loop() {
  timer.run();
}
