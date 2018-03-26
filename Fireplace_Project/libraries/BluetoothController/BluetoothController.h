/*
 * BluetoothController.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef BluetoothController_h
#define BluetoothController_h

#include "Arduino.h"

class BluetoothController {
  private:
    int btStatePin;
    int btMosfetPin;
    bool is_Connected;
    bool is_Enabled;

  public:
    BluetoothController(int btStatePinIn, int btMosfetPinIn);
    void setEnabled(bool enable);
    bool isEnabled();
    bool isConnected();
    void run();
};

#endif
