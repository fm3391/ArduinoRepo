int relayPin = 12;
int batterInputPin = A0;

class PowerModule{
  private:
  int relayCtrlPin;
  int batteryInputPin;
  boolean isCharging = false;
  const float battMaxAnalog = 700;
  const float battMaxVoltage = 11.75;
  float batteryVoltage;
  
  public:
  PowerModule(int relayCtrlPin, int batteryInputPin){
   this->relayCtrlPin = relayCtrlPin;
   this->batteryInputPin = batteryInputPin;
  }


  void enableCharging(){
    digitalWrite(this->relayCtrlPin, HIGH);
  }

  void disableCharging(){
    digitalWrite(this->relayCtrlPin, LOW);
  }
  
  void run(){
    batteryVoltage = ((float) analogRead(A0)/ battMaxAnalog) * battMaxVoltage;
    Serial.println("Battery Voltage: " + String(batteryVoltage));
    
    if(batteryVoltage >= 11.00){
      disableCharging();
      isCharging = false;
    }else if(batteryVoltage <= 9.60){
      enableCharging();
      isCharging = true;
    }
    

  }
  
};

PowerModule powerModule(relayPin, batterInputPin);

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      String inString = "";
      inString += (char)inChar;
      int val = inString.toInt();
      if(val == 0){
        powerModule.disableCharging();
      }else if(val == 1){
        powerModule.enableCharging();
      }
    }
  }
  powerModule.run();
  delay(1000);
}
