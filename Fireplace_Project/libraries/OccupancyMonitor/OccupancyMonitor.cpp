#include "OccupancyMonitor.h"

OccupancyMonitor::OccupancyMonitor(){

}

bool OccupancyMonitor::roomIsOccupied(){
	return isOccupied;
}

void OccupancyMonitor::activityDetected(){
	if(!isOccupied){
		isOccupied = true;
	}
	secondsSinceLastActivity = 0;
}

void OccupancyMonitor::run() {
	if(isOccupied){
	// Check if the room is inactive
		if(secondsSinceLastActivity >= maxNonActivitySeconds){
			isOccupied = false;
			secondsSinceLastActivity = 0;
		}else{
			secondsSinceLastActivity++;
		}
	}
}
