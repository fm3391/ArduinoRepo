/*
  OccupancyMonitor.h
*/
#ifndef OccupancyMonitor_h
#define OccupancyMonitor_h

class OccupancyMonitor {
  private:
		bool isOccupied = false;
    const int maxNonActivitySeconds = 300;
    int secondsSinceLastActivity = 0;

  public:
		OccupancyMonitor();
		bool roomIsOccupied();
		void activityDetected();
		void run();
};

#endif