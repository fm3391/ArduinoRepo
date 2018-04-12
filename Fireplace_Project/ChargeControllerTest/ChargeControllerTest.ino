#include <ChargeController.h>


ChargeController chargeController(12, A4);

void setup() {
   Serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
}

void loop() {
  chargeController.run();
  delay(1000);
}
