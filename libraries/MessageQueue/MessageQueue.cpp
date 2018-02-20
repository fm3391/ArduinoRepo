/*
 * MessageQueue.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "MessageQueue.h"

MessageQueue::MessageQueue() {
	this->msgCount = 0;
}

void MessageQueue::clearQueue() {
	for (int i = 0; i < queueSize; i++) {
		msgArray[i] = "";
	}
	msgCount = 0;
}

boolean MessageQueue::isEmpty() {
	boolean isEmpty = true;
	if (msgCount > 0) {
		isEmpty = false;
	}

	return isEmpty;
}

boolean MessageQueue::isFull() {
	boolean isQueueFull = false;
	if (msgCount == queueSize) {
		isQueueFull = true;
	}
	return isQueueFull;
}

int MessageQueue::getCount(){
	return msgCount;
}

void MessageQueue::enqueMsg(String msg) {
	if (!isFull()) {
		if (msgCount == 0) {
			msgArray[0] = msg;
		} else {
			msgArray[msgCount] = msg;
		}
		msgCount++;
	}
}

String MessageQueue::dequeMsg() {
	String msg = msgArray[0];
	if (msgCount == 1) {
		clearQueue();
	} else if (msgCount > 1) {
		for (int i = 1; i < msgCount; i++) {
			msgArray[i - 1] = msgArray[i];
		}
		msgArray[msgCount] = "";
		msgCount--;
	}

	return msg;
}