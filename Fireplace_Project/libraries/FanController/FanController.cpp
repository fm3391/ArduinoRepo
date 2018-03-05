#include "FanController.h"

FanController::FanController(int relayPin) {
  this->fanRelayPin = relayPin;
}

void FanController::startFan() {
  digitalWrite(this->fanRelayPin, HIGH);
}

void FanController::stopFan() {
  digitalWrite(this->fanRelayPin, LOW);
}