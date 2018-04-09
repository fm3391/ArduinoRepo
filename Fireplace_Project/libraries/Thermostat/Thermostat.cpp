#include "Thermostat.h"
#include "Arduino.h"

Thermostat::Thermostat(byte heatPinIn) {
		this->heatPin = heatPinIn;
}

bool Thermostat::isHeating(){
	return is_Heating;
}	

void Thermostat::run(){
	if(digitalRead(heatPin) == LOW && is_Heating){
		is_Heating = false;	
	}else if(digitalRead(heatPin) == HIGH && !is_Heating){
		is_Heating = true;
	}	
}
