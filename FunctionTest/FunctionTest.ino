#include <SimpleTimer.h>

// MD10C - Version: Latest
#include <MD10C.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// ValveController - Version: Latest
#include <ValveController.h>

int mosfetPin = 8;
int md10cDirPin = 7;
int md10cPwmPin = 9;

SimpleTimer timer;

ValveController valveController(md10cDirPin, md10cPwmPin, mosfetPin);

int relayPin = 13;
int batterInputPin = A0;

class ChargeController {
  private:
    int relayCtrlPin;
    int batteryInputPin;
    boolean isCharging = false;
    const float battMaxVoltage = 11.75;
    const int batteryMaxVal = 690;
    const int batteryMinVal = 625;
    const int chargeCounterMax = 15;
    int chargeCounter = 0;
  public:
    ChargeController(int relayCtrlPin, int batteryInputPin) {
      this->relayCtrlPin = relayCtrlPin;
      this->batteryInputPin = batteryInputPin;
    }

    float getVoltage(){
      float voltage = 0.00;
      if(isCharging){
        /* Disables charging if isCharging is true */
        disableCharging();
      }
      int batteryVal = analogRead(batteryInputPin);
      voltage = (((float)batteryVal / batteryMaxVal)*battMaxVoltage)*.035;
      if(isCharging){
        /* Re-enables charging if isCharging is true */
        enableCharging();
      }
      return voltage;
    }

    void enableCharging() {
      digitalWrite(this->relayCtrlPin, HIGH);
    }

    void disableCharging() {
      digitalWrite(this->relayCtrlPin, LOW);
    }

    void run() {
      // Checks the current state of charging
      if(isCharging){
        // Is charging
        if(chargeCounter == chargeCounterMax){
          disableCharging();
          isCharging = false;
          chargeCounter = 0;
        }else{
          chargeCounter++;
        }
        
      }else{
        // Is not charging
        int batteryVal = analogRead(batteryInputPin);
        float voltage = ((float)batteryVal / batteryMaxVal)*battMaxVoltage;
        if(batteryVal <= batteryMinVal){
          isCharging = true;
          enableCharging();
        }
      }
    }
};

ChargeController chargeController(relayPin, batterInputPin);


void runChargeController(){
  chargeController.run();
}


void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(mosfetPin, OUTPUT);
  pinMode(md10cDirPin, OUTPUT);
  pinMode(md10cPwmPin, OUTPUT);
  timer.setInterval(60000, runChargeController);
  runChargeController();
}

void loop() {
timer.run();
}


/*
 *   while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      String inString = "";
      inString += (char)inChar;
      int val = inString.toInt();
      if (val == 0) {
        powerModule.disableCharging();
      } else if (val == 1) {
        powerModule.enableCharging();
      } else if (val == 2) {
        
      } else if (val == 3) {
        
      }
    }
  }
  powerModule.run();
  delay(1000);
 */
