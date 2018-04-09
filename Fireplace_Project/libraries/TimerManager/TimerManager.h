/*
 * TimerManager.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef TimerManager_h
#define TimerManager_h

#include "QList.h"

class TimerManager {
  private:
    SimpleTimer *timer;
    QList<int> timerTypeList;
    QList<int> timerIdList;
		const int resetCnt = 1000;
    int timerCnt = 0;
		int runCounter = 0;

  public:
    TimerManager(SimpleTimer &timerIn) {
      this->timer = &timerIn;
    }

    void addTimer(int timerType, int timerId) {
      this->timer->disable(timerId);
      this->timerTypeList.push_back(timerType);
      this->timerIdList.push_back(timerId);
      timerCnt++;
    }
	
	void restartTimerById(int timerId){
		int index = 0;
		
		for (int i = 0; i < timerCnt; i++) {
			
		}
		
	}

	void restartAll(){
		for (int i = 0; i < timerCnt; i++) {
        this->timer->restartTimer(timerIdList.get(i));
      }
	}

    void enableAll() {
      for (int i = 0; i < timerCnt; i++) {
        this->timer->enable(timerIdList.get(i));
      }
    }

    void disableAll() {
      for (int i = 0; i < timerCnt; i++) {
        this->timer->disable(timerIdList.get(i));
      }
    }

    void enableTimerByType(int type) {
      this->timer->enable(timerIdList.get(type));
    }

    void disableTimerByType(int type) {
      this->timer->disable(timerIdList.get(type));
    }

    int count(){
      return timerCnt;
    }
		
		void run(){
			if(runCounter >= resetCnt){
				disableAll();
				restartAll();
				enableAll();
				
				runCounter = 0;
			}
			runCounter++;
		}
};


#endif
