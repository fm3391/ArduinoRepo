#include <ChargeController.h>
#include <SimpleTimer.h>


SimpleTimer timer;

int relayPin = 13;
int batterInputPin = A0;


ChargeController chargeController(relayPin, batterInputPin);

void runChargeController(){
  chargeController.run();
}


void setup() {
  Serial.begin(9600);
  timer.setInterval(5000, runChargeController);
  runChargeController();
}

void loop() {
timer.run();
}
