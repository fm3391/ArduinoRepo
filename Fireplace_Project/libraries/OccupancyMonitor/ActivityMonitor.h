/*
  ActivityMonitor.h
*/
#ifndef OccupancyMonitor_h
#define OccupancyMonitor_h

class ActivityMonitor {
  private:
		bool is_Active = false;
    const int maxNonActivitySeconds = 300;
    int secondsSinceLastActivity = 0;

  public:
		ActivityMonitor();
		bool isActive();
		void activityDetected();
		void run();
};

#endif