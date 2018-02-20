/*
 * MessageManager.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef MessageManager_h
#define MessageManager_h

#include "Arduino.h"
#include "MessageQueue.h"

class MessageManager {
  private:
	MessageQueue msgQueue;

  public:
	MessageManager();
	String getNextMessage();
	void addMessage(String msg);
	boolean messageAvailable();
	void run();
		
};

#endif
