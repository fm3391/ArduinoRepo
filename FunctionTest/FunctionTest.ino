// MD10C - Version: Latest
#include <MD10C.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// ValveController - Version: Latest
#include <ValveController.h>

int mosfetPin = 8;
int md10cDirPin = 7;
int md10cPwmPin = 9;

ValveController valveController(md10cDirPin, md10cPwmPin, mosfetPin);

int relayPin = 13;
int batterInputPin = A0;

class PowerModule {
  private:
    int relayCtrlPin;
    int batteryInputPin;
    boolean isCharging = false;
    const float battMaxAnalog = 700;
    const float battMaxVoltage = 11.75;
    float batteryVoltage;

  public:
    PowerModule(int relayCtrlPin, int batteryInputPin) {
      this->relayCtrlPin = relayCtrlPin;
      this->batteryInputPin = batteryInputPin;
    }


    void enableCharging() {
      digitalWrite(this->relayCtrlPin, HIGH);
    }

    void disableCharging() {
      digitalWrite(this->relayCtrlPin, LOW);
    }

    float getBatteryVoltage(){
      batteryVoltage = ((float) analogRead(A0) / battMaxAnalog) * battMaxVoltage;
      return batteryVoltage;
    }

    void run() {
      
      Serial.println("Battery Voltage: " + String(batteryVoltage));
      getBatteryVoltage();
      if (batteryVoltage >= 11.00) {
        disableCharging();
        isCharging = false;
      } else if (batteryVoltage <= 9.60) {
        enableCharging();
        isCharging = true;
      }


    }

};

PowerModule powerModule(relayPin, batterInputPin);



class CommManager{
  private:


  public:
  
};

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
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
      if (val == 0) {
        powerModule.disableCharging();
      } else if (val == 1) {
        powerModule.enableCharging();
      } else if (val == 2) {
        valveController.closeValve((int)powerModule.getBatteryVoltage());
      } else if (val == 3) {
        valveController.openValve((int)powerModule.getBatteryVoltage());
      }
    }
  }
  powerModule.run();
  delay(1000);
}
