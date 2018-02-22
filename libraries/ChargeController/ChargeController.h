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
		const float battMaxVoltage = 10.20;
		const float battMinVoltage = 9.60;
		int chargeCounter = 0;
		const int chargeCounterMax = 10;
		float voltage = 0.00;

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
		void run();

};

#endif
