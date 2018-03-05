/*
 * ActivityMonitor.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef ActivityMonitor_h
#define ActivityMonitor_h

#include "Arduino.h"

class ActivityMonitor {
  private:
    bool isActive = false;
    unsigned long lastActiveTime;
    unsigned long maxInActivityTime = (long) 20000;
	
  public:
	ActivityMonitor();
	void activityDetected();
	bool getIsActive();
	void setInActiveTime(long inActiveTime);
	void run();
	
};

#endif
