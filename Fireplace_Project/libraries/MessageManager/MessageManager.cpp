/*
 * MessageManager.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "MessageManager.h"
#include "QList.h"
#include "SimpleQueue.h"

MessageManager::MessageManager() {
  // NO-OP
}

void MessageManager::addOutboundMsg(String msg){
	outboundMsgQueue.push_back(msg);
}
String MessageManager::getInboundMessage(){
	// Get the message from the front
	String msg = inboundMsgQueue.front();
	// Remove it from the list
	inboundMsgQueue.pop_front();
	// Return the message
	return msg;
}

bool MessageManager::availableInboundMsg(){
	bool msgAvailable = false;
	if(inboundMsgQueue.size() > 0){
		msgAvailable = true;
	}
	return msgAvailable;
}

void MessageManager::parseMessage(String msg, SimpleQueue &queue){
	String identifier = "";

      for (int i = 0; i < msg.length(); i++) {
        if (i == msg.length()-1) {
          identifier = identifier + msg.charAt(i);
          queue.push(identifier);
          break;
        }
        else if (msg.charAt(i) == ':') {
          queue.push(identifier);
          identifier = EMPTY;
        } else {
          identifier = identifier + msg.charAt(i);
        }
      }
}



void MessageManager::run() {
	if(outboundMsgQueue.size() > 0){
		String outboundMsg = outboundMsgQueue.front();
		outboundMsgQueue.pop_front();
		outboundMsg = outboundMsg + EOM;
		Serial.println(outboundMsg);
	}
	
	if (Serial.available() > 0) {
	  String msg = EMPTY;
	  while(Serial.available() > 0){
		 char nextChar = Serial.read();
		 if(nextChar == EOM){
			inboundMsgQueue.push_back(msg); 
			msg = EMPTY;
		 }else{
			 msg.concat(nextChar);
		 }	
	  }
  }
}