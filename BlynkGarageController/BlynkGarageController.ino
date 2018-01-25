#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "34d1777241174d91b97afb9a77dd55fa";

char ssid[] = "NETGEAR36_ATT";
char pass[] = "asdf1234ASDF!@#$";

const int relayPin = 16;
const int sensorOpenPin = 5;
const int sensorClosedPin = 4;

enum class Color {RED, GREEN, BLUE};

Color color;

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue == 1) {
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
  }
}

BLYNK_READ(V0)
{
  int sensorOpenVal = digitalRead(sensorOpenPin);
  int sensorClosedVal = digitalRead(sensorClosedPin);
  String state = "";

  if (sensorOpenVal == 1) {
    state = "OPEN";
  } else if (sensorClosedVal == 1) {
    state = "CLOSED";
  } else {
    state = "TRANSISTIONING";
  }
  Serial.println("State: " + state);
  Blynk.virtualWrite(V0, state);
}


void setup() {
  Blynk.begin(auth, ssid, pass);

  pinMode(relayPin, OUTPUT);
  pinMode(sensorOpenPin, INPUT);
  pinMode(sensorClosedPin, INPUT);

  digitalWrite(relayPin, HIGH);
}

void loop() {
  Blynk.run();
}
