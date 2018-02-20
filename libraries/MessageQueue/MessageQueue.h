/*
 * MessageQueue.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef MessageQueue_h
#define MessageQueue_h

#include "Arduino.h"

class MessageQueue {
  private:
    static const int queueSize = 10;
    String msgArray[queueSize] = {};
    int msgCount;

  public:
    MessageQueue();
		void clearQueue();
		boolean isEmpty();
		boolean isFull();
		int getCount();
		void enqueMsg(String msg);
		String dequeMsg();
		void printQueue();
		
};

#endif
