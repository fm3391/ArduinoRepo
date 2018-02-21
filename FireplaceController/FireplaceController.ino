#include <ValveControllerState.h>

// MessageManager - Version: Latest
#include <MessageManager.h>

// MessageQueue - Version: Latest
#include <MessageQueue.h>

// ChargeController - Version: Latest
#include <ChargeController.h>

// ValveController - Version: Latest
#include <ValveController.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// MD10C - Version: Latest
#include <MD10C.h>

// SimpleTimer - Version: Latest
#include <SimpleTimer.h>

/*

*/


// Pin Declarations
int vcDirPin = 7; // ValveController DIR pin
int vcPwmPin = 9; // ValveController PWM pin
int vcMosfetPin = 8; // ValveController Mosfet pin

int ccRelayPin = 12; // ChargeController Relay pin
int ccBattInputPin = A0; // ChargeController Input pin

SimpleTimer timer;
MessageManager messageManager;
ChargeController chargeController(ccRelayPin, ccBattInputPin);

class FireplaceController {
  private:
    MessageManager *messageManager;
    ChargeController *chargeController;
    ValveController valveController;

  public:
    FireplaceController(MessageManager &messageManagerIn, ChargeController &chargeControllerIn)
      : valveController(vcDirPin, vcPwmPin, vcMosfetPin) {
      this->messageManager = &messageManagerIn;
      this->chargeController = &chargeControllerIn;
    }

    void run() {
    chargeController->updateVoltage();

    }
};

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

void runTest() {
  chargeController.updateVoltage();
}

void setup() {
  Serial.begin(38400); 
  pinMode(ccRelayPin, OUTPUT);
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);
  digitalWrite(ccRelayPin, LOW);
  digitalWrite(vcMosfetPin, LOW);


  chargeController.run();
  messageManager.run();

  //timer.setInterval(500, runMessageManager);
  //timer.setInterval(1000, runFireplaceController);
  timer.setInterval(60000, runChargeController);
  timer.setInterval(2000, runTest);
}

void loop() {
  timer.run();
}
