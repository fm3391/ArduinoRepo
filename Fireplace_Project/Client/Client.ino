#include <Enums.h>
#include <SimpleTimer.h>
#include <SimpleQueue.h>
#include <MessageManager.h>



const int ledPin = 13;

SimpleTimer timer;
MessageManager messageManager;


void runTimeout(){
  digitalWrite(ledPin, LOW);
}

void processMessages() {
  while (messageManager.availableInboundMsg()) {
    String msg = messageManager.getInboundMessage();
    SimpleQueue tmpQueue;
    messageManager.parseMessage(msg, tmpQueue);
    MessageType msgType = (MessageType) (tmpQueue.elementAt(0)).toInt();
    if(msgType == MessageType::HRBT){
      digitalWrite(ledPin, HIGH);
      timer.setTimeout(500, runTimeout);
    }
    
  }
}


void runMessageManager() {
  messageManager.run();
}

void setup() {
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  timer.setInterval(500, runMessageManager);
  timer.setInterval(750, processMessages);
}

void loop() {
  timer.run();
}
