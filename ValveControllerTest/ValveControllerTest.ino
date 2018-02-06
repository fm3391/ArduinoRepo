// MD10C - Version: Latest
#include <MD10C.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// ValveController - Version: Latest
#include <ValveController.h>

int mosfetPin = 12;
int md10cDirPin = 5;
int md10cPwmPin = 6;

ValveController valveController(md10cDirPin, md10cPwmPin, mosfetPin);

void setup() {
  Serial.begin(9600);
  pinMode(mosfetPin, OUTPUT);
  pinMode(md10cDirPin, OUTPUT);
  pinMode(md10cPwmPin, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      String inString = "";
      inString += (char)inChar;
      int val = inString.toInt();
      if(val == 0){
        valveController.closeValve(11);
      }else if(val == 1){
        valveController.openValve(11);
      }
    }
  }
}
