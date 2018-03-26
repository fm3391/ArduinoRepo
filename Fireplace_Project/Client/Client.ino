#include <Thermostat.h>

#include <BluetoothController.h>

// Libraries
#include <Enums.h>
#include <MessageManager.h>
#include <SimpleTimer.h>

// Constants
const int heartBtLedPin = 12;
const int btStatePin = 6; // Pin that indicate connection status
const int heatPin = 7;
const int coolPin = 8;

SimpleTimer timer;
MessageManager messageManager;
BluetoothController btController(btStatePin, -99);
Thermostat thermostat(heatPin, coolPin);

void runTimeout() {
  digitalWrite(heartBtLedPin, LOW);
}

class MainApp {
  private:


  public:

    void run() {
      String thermostatMode = "";
      if (thermostat.getMode() == ThermostatMode::OFF) {
        thermostatMode = "OFF";
      } else if (thermostat.getMode() == ThermostatMode::HEATING) {
        thermostatMode = "HEATING";
      } else if (thermostat.getMode() == ThermostatMode::COOLING) {
        thermostatMode = "COOLING";
      }

      Serial.println(thermostatMode);
    }
};

MainApp mainApp;

void processMessages() {
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    if (msgType == MessageType::HRBT) {
      digitalWrite(heartBtLedPin, HIGH);
      timer.setTimeout(500, runTimeout);
    }
  }

}
void runMainApp() {
  mainApp.run();
}
void runThermostat() {
  thermostat.run();
}

void runBluetoothController() {
  btController.run();
}

void runMessageManager() {
  messageManager.run();
}

void setup() {
  Serial.begin(38400);

  // Set OUTPUT pinModes
  pinMode(heartBtLedPin, OUTPUT);


  // Set INPUT pinModes
  pinMode(btStatePin, INPUT);
  pinMode(heatPin, INPUT);
  pinMode(coolPin, INPUT);

  timer.setInterval(500, processMessages);
  timer.setInterval(500, runMessageManager);
  timer.setInterval(1000, runBluetoothController);
  timer.setInterval(1000, runThermostat);
  timer.setInterval(1000, runMainApp);
}

void loop() {
  timer.run();
}
