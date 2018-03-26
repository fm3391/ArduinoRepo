/*
 * Mosfet.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */
#include "Arduino.h"
#include "BluetoothController.h"

 BluetoothController::BluetoothController(int btStatePinIn, int btMosfetPinIn) {
      this->btStatePin = btStatePinIn;
      this->btMosfetPin = btMosfetPinIn;
      this->is_Connected = false;
      this->is_Enabled = false;
    }

    void BluetoothController::setEnabled(bool enable) {
      if (enable) {
        is_Enabled = true;
        digitalWrite(this->btMosfetPin, HIGH);
      } else {
        is_Enabled = false;
        digitalWrite(this->btMosfetPin, LOW);
      }
    }

    bool BluetoothController::isEnabled() {
      return is_Enabled;
    }

    bool BluetoothController::isConnected() {
      return is_Connected;
    }

    void BluetoothController::run() {
      int btStatePinVal = digitalRead(this->btStatePin);
      if(btStatePinVal == 1 && !is_Connected){
          is_Connected = true;      
      }else if(btStatePinVal == 0 && is_Connected){
          is_Connected = false;
      }
    }


