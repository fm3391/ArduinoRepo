#include "ActivityMonitor.h"
#include "Arduino.h"

ActivityMonitor::ActivityMonitor(int pirPinIn){
this->pirPin = pirPinIn;
}

bool ActivityMonitor::isActive(){
	return is_Active;
}

void ActivityMonitor::run() {
	if(digitalRead(this->pirPin) == HIGH){
		if(!is_Active){
			is_Active = true;
		}
		counter = 0;
	}
	if(is_Active){
		counter++;
		if(counter >= counterMax){
			counter = 0;
			is_Active = false;
		}
	}
}
