/*
  ActivityMonitor.h
*/
#ifndef OccupancyMonitor_h
#define OccupancyMonitor_h

#include "Arduino.h"

class ActivityMonitor {
  private:
		int pirPin;
		bool is_Active = false;
		int counter = 0;
		const int counterMax = 100;

  public:
		ActivityMonitor(int pirPinIn);
		bool isActive();
		void activityDetected();
		void run();
};

#endif