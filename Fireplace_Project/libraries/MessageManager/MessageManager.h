/*
 * MessageManager.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef MessageManager_h
#define MessageManager_h

#include "Arduino.h"
#include "QList.h"
#include "SimpleQueue.h"

class MessageManager {
  private:
	QList<String> inboundMsgQueue;
	QList<String> outboundMsgQueue;
	const char EOM = '$';
	const String EMPTY = "";
  public:
	MessageManager();
	void addOutboundMsg(String msg);
	String getInboundMessage();
	bool availableInboundMsg();
	void parseMessage(String msg, SimpleQueue &queue);
	void run();
		
};

#endif
