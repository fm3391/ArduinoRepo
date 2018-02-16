// ChargeController - Version: Latest
#include <ChargeController.h>

// ValveController - Version: Latest
#include <ValveController.h>

// Mosfet - Version: Latest
#include <Mosfet.h>

// MD10C - Version: Latest
#include <MD10C.h>

// SimpleTimer - Version: Latest
#include <SimpleTimer.h>

/*

*/

SimpleTimer timer;

#define vcDirPin 0
#define vcPwmPin 0
#define vcMosfetPin 0

#define ccRelayPin 0
#define ccBattInputPin 0

enum class Mode {
  TIMED,
  LOW_POWER,
  NORMAL
};



class MessageQueue {
  private:
    static const int queueSize = 10;
    String msgArray[queueSize] = {};
    int msgCount;

  public:
    MessageQueue() {
      this->msgCount = 0;
    }

    void clearQueue() {
      for (int i = 0; i < queueSize; i++) {
        msgArray[i] = "";
      }
      msgCount = 0;
    }

    boolean isEmpty() {
      boolean isEmpty = true;
      if (msgCount > 0) {
        isEmpty = false;
      }

      return isEmpty;
    }

    boolean isFull() {
      boolean isQueueFull = false;
      if (msgCount == 5) {
        isQueueFull = true;
      }
      return isQueueFull;
    }

    int getCount(){
      return msgCount;
    }

    void enqueMsg(String msg) {
      if (!isFull()) {
        if (msgCount == 0) {
          msgArray[0] = msg;
        } else {
          msgArray[msgCount] = msg;
        }
        msgCount++;
        
      }
    }

    String dequeMsg() {
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

    void printQueueArray() {
      for (int i = 0; i < msgCount; i++) {
        Serial.println(msgArray[i]);
      }
    }

};



class MessageManager {
  private:
    MessageQueue msgQueue;
  public:
    MessageManager() {
      // NO-OP
    }

    String getNextMessage() {
      return msgQueue.dequeMsg();
    }

    void addMessage(String msg) {
      msgQueue.enqueMsg(msg);
    }

    boolean messageAvailable() {
      return !msgQueue.isEmpty();
    }


    void run() {
      if (Serial.available() > 0) {
        String message = "";
        while (Serial.available() > 0) {
          char nextChar = Serial.read();
          if (nextChar == '$') {
            msgQueue.enqueMsg(message);
          } else {
            message.concat(nextChar);
          }
        }
        message = "";
      }
    }

};


MessageManager messageManager;
ChargeController chargeController(ccRelayPin, ccBattInputPin);
ValveController valveController(vcDirPin, vcPwmPin, vcMosfetPin);

class FireplaceController {
  private:
    MessageManager *messageManager;
    Mode mode;


  public:
    FireplaceController(MessageManager &messageManager) {
      this->mode = Mode::NORMAL;
      this->messageManager = &messageManager;
    }

    void run() {
      while(messageManager->messageAvailable()){
        Serial.println(messageManager->getNextMessage());
      }
    }
};

FireplaceController fireplaceController(messageManager);

void runFireplaceController() {
  fireplaceController.run();
}

void runMessageManager() {
  messageManager.run();
}

void runChargeController() {
  //chargeController.run();
}

void setup() {
  Serial.begin(38400);
  timer.setInterval(1000, runMessageManager);
  timer.setInterval(10000, runFireplaceController);
}

void loop() {
  timer.run();
}
