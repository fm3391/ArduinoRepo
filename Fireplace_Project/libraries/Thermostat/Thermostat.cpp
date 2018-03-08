#include "Thermostat.h"
#include "Arduino.h"
#include "Enums.h"

Thermostat::Thermostat(int heatPin, int coolPin) {
		this->heatPin = heatPin;
		this->coolPin = coolPin;
}

ThermostatMode Thermostat::getMode(){
	return this->mode;
}

void Thermostat::run(){
	if(digitalRead(heatPin) == HIGH && mode != ThermostatMode::HEATING){
		mode = ThermostatMode::HEATING;
	}else if(digitalRead(coolPin) == HIGH && mode != ThermostatMode::COOLING){
		mode = ThermostatMode::COOLING;
	}else if(digitalRead(heatPin) == LOW && 
			digitalRead(coolPin) == LOW && mode != ThermostatMode::OFF){
		mode = ThermostatMode::OFF;
	}
}
