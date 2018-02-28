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

void MessageManager::run() {
	
  if(!outboundMsgQueue.empty()){
	String outboundMsg = outboundMsgQueue.pop();
	outboundMsg = outboundMsg + "$";
	Serial.println(outboundMsg);
  }
	
  if (Serial.available() > 0) {
	  String msg = "";
	  while(Serial.available() > 0){
		 char nextChar = Serial.read();
		 if(nextChar == '$'){
			inboundMsgQueue.push(msg); 
			msg = "";
		 }else{
			 msg.concat(nextChar);
		 }	
	  }
  }
	
  
}