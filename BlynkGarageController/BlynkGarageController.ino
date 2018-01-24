#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "34d1777241174d91b97afb9a77dd55fa";

char ssid[] = "NETGEAR36_ATT";
char pass[] = "asdf1234ASDF!@#$";

int relayPin = 16;
int sensorOpenPin = 5;
int sensorClosedPin = 4;

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(pinValue == 1){
    digitalWrite(relayPin, LOW);
  }else{
    digitalWrite(relayPin, HIGH);
  }
}


void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(relayPin,OUTPUT);
  pinMode(sensorOpenPin,INPUT);
  pinMode(sensorClosedPin,INPUT);

  digitalWrite(relayPin, HIGH);
}

void loop() {
  Blynk.run();
  Serial.print("Sensor 1: "+ String(digitalRead(sensorOpenPin)));
  Serial.print(" / ");
  Serial.println("Sensor 2: "+ String(digitalRead(sensorClosedPin)));
  delay(1000);
}
