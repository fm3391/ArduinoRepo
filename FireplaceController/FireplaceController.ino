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

SimpleTimer timer;

int vcDirPin = 7;
int vcPwmPin = 9;
int vcMosfetPin = 8;

int ccRelayPin = 12;
int ccBattInputPin = A0;


// Controller Interval Values
#define chargeControllerInterval 60
#define messageManagerInterval 0.5
#define fireplaceControllerInterval 1

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

void setup() {
  Serial.begin(9600);
  pinMode(ccRelayPin, OUTPUT);
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);
  digitalWrite(ccRelayPin, LOW);
  digitalWrite(vcMosfetPin, LOW);
  //timer.setInterval(messageManagerInterval * 1000, runMessageManager);
  timer.setInterval(fireplaceControllerInterval * 1000, runFireplaceController);
  chargeController.run();
  timer.setInterval(chargeControllerInterval * 1000, runChargeController);
}

void loop() {
  timer.run();
}
