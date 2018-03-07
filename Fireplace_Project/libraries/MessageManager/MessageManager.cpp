/*
 * MessageManager.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "MessageManager.h"
#include "SimpleQueue.h"

MessageManager::MessageManager() {
  // NO-OP
}

void MessageManager::addOutboundMsg(String msg){
	outboundMsgQueue.push(msg);
}
String MessageManager::getInboundMessage(){
	return inboundMsgQueue.pop();
}

bool MessageManager::availableInboundMsg(){
	bool msgAvailable = true;
	if(inboundMsgQueue.empty()){
		msgAvailable = false;
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

void MessageManager::clearInboundMsgs(){
	inboundMsgQueue.clear();
}

void MessageManager::clearOutboundMsgs(){
	outboundMsgQueue.clear();
}

void MessageManager::run() {
	
  if(!outboundMsgQueue.empty()){
	String outboundMsg = outboundMsgQueue.pop();
	outboundMsg = outboundMsg + EOM;
	Serial.println(outboundMsg);
  }
	
  if (Serial.available() > 0) {
	  String msg = EMPTY;
	  while(Serial.available() > 0){
		 char nextChar = Serial.read();
		 if(nextChar == EOM){
			inboundMsgQueue.push(msg); 
			msg = EMPTY;
		 }else{
			 msg.concat(nextChar);
		 }	
	  }
  }
}