/*
 * MessageManager.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef MessageManager_h
#define MessageManager_h

#include "Arduino.h"
#include "SimpleQueue.h"

class MessageManager {
  private:
	SimpleQueue inboundMsgQueue;
	SimpleQueue outboundMsgQueue;

  public:
	MessageManager();
	void addOutboundMsg(String msg);
	String getInboundMessage();
	bool availableInboundMsg();
	void run();
		
};

#endif
