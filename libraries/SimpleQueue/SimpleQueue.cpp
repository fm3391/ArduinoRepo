/*
 * SimpleQueue.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "SimpleQueue.h"

SimpleQueue::SimpleQueue() {
	this->elementCount = 0;
}

String SimpleQueue::elementAt(int i){
	return elementArray[i];
}

void SimpleQueue::clear() {
	for (int i = 0; i < queueSize; i++) {
		elementArray[i] = "";
	}
	elementCount = 0;
}

boolean SimpleQueue::empty() {
	boolean isEmpty = true;
	if (elementCount > 0) {
		isEmpty = false;
	}
	return isEmpty;
}

boolean SimpleQueue::full() {
	boolean isQueueFull = false;
	if (elementCount == queueSize) {
		isQueueFull = true;
	}
	return isQueueFull;
}

int SimpleQueue::count(){
	return elementCount;
}

void SimpleQueue::push(String val) {
	
	if (!full()) {
		if (elementCount == 0) {
			elementArray[0] = val;
		} else {
			elementArray[elementCount] = val;
		}
		elementCount++;
	}
}

String SimpleQueue::pop() {
	String element = elementArray[0];
	if (elementCount == 1) {
		clear();
	} else if (elementCount > 1) {
		for (int i = 1; i < elementCount; i++) {
			elementArray[i - 1] = elementArray[i];
		}
		elementArray[elementCount] = "";
		elementCount--;
	}

	return element;
}