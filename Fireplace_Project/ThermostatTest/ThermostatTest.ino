#include <Thermostat.h>


const int inputPin = 2;

Thermostat thermostat(inputPin);

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);

}

void loop() {
  thermostat.run();
  Serial.println("Is Heating: " + String(thermostat.isHeating()));
  delay(1000);
}
