/*
 * ActivityMonitor.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "ActivityMonitor.h"

ActivityMonitor::ActivityMonitor(){
	lastActiveTime = millis();
}


void ActivityMonitor::activityDetected() {
      lastActiveTime = millis();
      if (!isActive) {
        isActive = true;
      }
}

bool ActivityMonitor::getIsActive() {
      return isActive;
}
void ActivityMonitor::run() {
		
      if (millis() < lastActiveTime) {
        lastActiveTime = millis();
      } else if ((millis() - lastActiveTime) > maxInActivityTime) {
        isActive = false;
      }
}

void ActivityMonitor::setInActiveTime(long inActiveTime){
	maxInActivityTime = (long) inActiveTime;
}