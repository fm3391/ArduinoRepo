#include <ValveControllerState.h>
#include <MessageManager.h>
#include <MessageQueue.h>
#include <ChargeController.h>
#include <ValveController.h>
#include <Mosfet.h>
#include <MD10C.h>
#include <SimpleTimer.h>
#include <BatteryStatus.h>

/*

*/

// Pin Declarations
const int vcDirPin = 7; // ValveController DIR pin
const int vcPwmPin = 9; // ValveController PWM pin
const int vcMosfetPin = 8; // ValveController Mosfet pin

const int ccRelayPin = 12; // ChargeController Relay pin
const int ccBattInputPin = A0; // ChargeController Input pin

const int btMosfetPin = 5; // Mosfet that controls HC-05 module
const int btStatePin = 6; // Pin that indicate connection status


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
      this->fireplaceStatus = FireplaceStatus::OFF;
      this->mode = SystemMode::NORMAL;
      this->messageManager->run();
      this->chargeController->run();
    }

  public:
    FireplaceController(MessageManager &messageManagerIn, ChargeController &chargeControllerIn)
      : valveController(vcDirPin, vcPwmPin, vcMosfetPin) {
      this->messageManager = &messageManagerIn;
      this->chargeController = &chargeControllerIn;
      init();
    }

    void run() {
      while(messageManager->messageAvailable()){
        String msg = messageManager->getNextMessage();

        Serial.println("Processed Message: "+msg);
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
