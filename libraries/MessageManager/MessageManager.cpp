/*
 * MessageManager.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "MessageManager.h"
#include "MessageQueue.h"

MessageManager::MessageManager() {
  // NO-OP
}

String MessageManager::getNextMessage() {
	String nextMsg = msgQueue.dequeMsg();
	return nextMsg;
}

void MessageManager::addMessage(String msg) {
	if(!msgQueue.isFull()){
		msgQueue.enqueMsg(msg);
	}
}

boolean MessageManager::messageAvailable() {
  return !msgQueue.isEmpty();
}


void MessageManager::run() {
  String msg = "";
  if (Serial.available() > 0) {
	  while(Serial.available() > 0){
		 char nextChar = Serial.read();
		 if(nextChar == '$'){
			addMessage(msg);
			msg = "";
		 }else{
			 msg.concat(nextChar);
		 }	
	  }
  }
	
  
}