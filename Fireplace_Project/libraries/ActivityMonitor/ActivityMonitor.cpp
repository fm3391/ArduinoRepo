#include "ActivityMonitor.h"

ActivityMonitor::ActivityMonitor(){

}

bool ActivityMonitor::isActive(){
	return is_Active;
}

void ActivityMonitor::activityDetected(){
	if(!is_Active){
		is_Active = true;
	}
	secondsSinceLastActivity = 0;
}

void ActivityMonitor::run() {
	if(is_Active){
	// Check if the room is inactive
		if(secondsSinceLastActivity >= maxNonActivitySeconds){
			is_Active = false;
			secondsSinceLastActivity = 0;
		}else{
			secondsSinceLastActivity++;
		}
	}
}
