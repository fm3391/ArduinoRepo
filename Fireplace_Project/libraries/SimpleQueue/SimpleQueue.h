/*
 * SimpleQueue.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef SimpleQueue_h
#define SimpleQueue_h

#include "Arduino.h"

class SimpleQueue {
  private:
    static const int queueSize = 10;
    String elementArray[queueSize] = {};
    int elementCount;

  public:
    SimpleQueue();
	void clear();
	boolean empty();
	boolean full();
	int count();
	void push(String element);
	String pop();
	String elementAt(int i);
		
};

#endif
