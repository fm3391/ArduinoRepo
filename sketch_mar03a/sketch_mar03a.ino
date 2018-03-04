#include <MD10C.h>

#include <Mosfet.h>

#include <ValveController.h>

const int vcDirPin = 7; // ValveController DIR pin
const int vcPwmPin = 9; // ValveController PWM pin
const int vcMosfetPin = 8; // ValveController Mosfet pin


ValveController valveController(vcDirPin, vcPwmPin, vcMosfetPin);
void setup() {
  pinMode(vcMosfetPin, OUTPUT);
  pinMode(vcDirPin, OUTPUT);
  pinMode(vcPwmPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(vcMosfetPin, HIGH);
  delay(1000);
  digitalWrite(vcMosfetPin, LOW);
  delay(1000);

}
