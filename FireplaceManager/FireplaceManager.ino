// MD10C - Version: Latest
#include <MD10C.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// ValveController - Version: Latest
#include <ValveController.h>

int mosfetPin = 12;
int md10cDirPin = 5;
int md10cPwmPin = 6;





void setup() {
  pinMode(mosfetPin, OUTPUT);
  pinMode(md10cDirPin, OUTPUT);
  pinMode(md10cPwmPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}
