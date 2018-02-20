#include <SimpleTimer.h>

// MessageManager - Version: Latest
#include <MessageManager.h>

// MessageQueue - Version: Latest
#include <MessageQueue.h>

SimpleTimer timer;
MessageManager messageManager;


void runMessageManager() {
  messageManager.run();
} 

void testRunner(){
  if(messageManager.messageAvailable()){
    Serial.println(messageManager.getNextMessage());
  }
}

void setup() {
  Serial.begin(38400);
  timer.setInterval(500, runMessageManager);
  timer.setInterval(3000, testRunner);
}

void loop() {
  timer.run();
}

