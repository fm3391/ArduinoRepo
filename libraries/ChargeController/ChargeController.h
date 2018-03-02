/*
 * ChargeController.h
 *
 *  Created on: Jul 14, 2017
 *      Author: scottles
 */

#ifndef ChargeController_h
#define ChargeController_h

#include "Arduino.h"

class ChargeController {
	private:
		int relayCtrlPin;
		int batteryInputPin;
		bool isCharging = false;
		const float nominalVoltage = 11.50;
		const float minVoltage = 10.00;
		int chargeCounter = 0;
		const int chargeCounterMax = 20;
		float voltage = 0.00;
		int batteryLevel = 0;

	void enableCharging(){
		digitalWrite(this->relayCtrlPin, HIGH);
	}
	void disableCharging(){
		digitalWrite(this->relayCtrlPin, LOW);
	}

	public:
		ChargeController(int relayCtrlPin, int batteryInputPin);
		float getVoltage();
		void updateVoltage();
		int getBatteryLevel();
		void run();

};

#endif
