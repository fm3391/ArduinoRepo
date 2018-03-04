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

  References:
  - https://www.baldengineer.com/state-machine-with-enum-tutorial.html
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

const String EMPTY = "";
const String SEPERATOR = ":";

bool isConnected = false;

/*

*/

class FireplaceController {
  private:
    SystemMode mode = SystemMode::NORMAL;
    MessageManager *messageManager;
    ChargeController *chargeController;
    ValveController valveController;
    FireplaceStatus fireplaceStatus;
    bool isInit = false;

    void startFireplace() {
      valveController.openValve(chargeController->getVoltage());
      fireplaceStatus = FireplaceStatus::RUNNING;
    }

    void stopFireplace() {
      valveController.closeValve(chargeController->getVoltage());
      fireplaceStatus = FireplaceStatus::OFF;
    }

  public:
    FireplaceController(MessageManager &messageManagerIn, ChargeController &chargeControllerIn)
      : valveController(vcDirPin, vcPwmPin, vcMosfetPin) {
      this->messageManager = &messageManagerIn;
      this->chargeController = &chargeControllerIn;

    }

    void handleCmdMsg(MessageSpecifier &msgSpec, MessageCmd msgCmd){
      if(msgCmd == MessageCmd::FIRE_ON){
        if(fireplaceStatus != FireplaceStatus::RUNNING){
          startFireplace();
        }
      }else if(msgCmd == MessageCmd::FIRE_OFF){
        if(fireplaceStatus == FireplaceStatus::RUNNING){
          stopFireplace();
        }
      }
      handleReqMsg(MessageSpecifier::FIRE);
    }

    void handleReqMsg(MessageSpecifier msgSpec){
      String msgOut = EMPTY;
      String tmpSpecifier = EMPTY;
      String tmpValue = EMPTY;
      switch(msgSpec){
        case MessageSpecifier::BATT:   
          tmpSpecifier = String((int)MessageSpecifier::BATT);
          tmpValue = String((int) chargeController->getBatteryStatus());
        break;
        case MessageSpecifier::FIRE:
          tmpSpecifier = String((int)MessageSpecifier::FIRE);
          tmpValue = String((int) fireplaceStatus);
        break;
      }
      
      if(tmpSpecifier != EMPTY && tmpValue != EMPTY){   
        msgOut = String((int) MessageType::INFO) + SEPERATOR 
                 + tmpSpecifier + SEPERATOR + tmpValue;
        messageManager->addOutboundMsg(msgOut);
      }
    }

    void run() {
      if (!isInit) {
        chargeController->run();
        messageManager->run();
        stopFireplace();
        //if(isConnected){
        if(true){  
          handleReqMsg(MessageSpecifier::BATT);
          delay(250);
          handleReqMsg(MessageSpecifier::FIRE);
        }
        isInit = true;
      }
    }
};

// Object Instantiations
SimpleTimer timer;
MessageManager messageManager;
ChargeController chargeController(ccRelayPin, ccBattInputPin);
FireplaceController fireplaceController(messageManager, chargeController);


void processMessages() {
  if (true) {
    //if (isConnected()) {
    while (messageManager.availableInboundMsg()) {
      String msg = messageManager.getInboundMessage();
      SimpleQueue tmpQueue;
      messageManager.parseMessage(msg, tmpQueue);
      MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
      MessageSpecifier msgSpec = (MessageSpecifier)(tmpQueue.elementAt(1)).toInt();
      
      switch(msgType){
        case MessageType::REQ:
          fireplaceController.handleReqMsg(msgSpec);
        break;
        case MessageType::CMD:
          MessageCmd cmd = (MessageCmd) (tmpQueue.elementAt(2)).toInt();
          fireplaceController.handleCmdMsg(msgSpec, cmd);
        break;
      }

    }
  } else if (messageManager.availableInboundMsg()) {
    messageManager.clearInboundMsgs();
  }
}

void runFireplaceController() {
  fireplaceController.run();
}

void runMessageManager() {
  messageManager.run();
}

void runChargeController() {
  chargeController.run();
}

void runConnectionUpdate(){
  int connectionVal = digitalRead(btStatePin); 
  if(isConnected && connectionVal == LOW){
    isConnected = false;
  }else if(!isConnected && connectionVal == HIGH){
    isConnected = true;
  }
}

void setup() {
  Serial.begin(9600);

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
  digitalWrite(btMosfetPin, HIGH);

  // Update Connection Status
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
