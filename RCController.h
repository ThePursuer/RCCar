/*
 * RCController.h
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#ifndef RCCONTROLLER_H_
#define RCCONTROLLER_H_
#include "RCCar.h"

#include <memory>

#include <linux/joystick.h>

class RC_Controller {
public:
	RC_Controller(std::shared_ptr<RC_Car> rc);

	void handleJoystickEvent(js_event event);
private:
	struct joystickTriggers{
		int16_t rightTrigger = INT16_MIN;
		int16_t leftTrigger = INT16_MIN;
	} js_;

	struct buttons{
		bool leftBumper = false;
		bool rightBumper = false;
	} buttons_;

	std::shared_ptr<RC_Car> rc_;
};

#endif /* RCCONTROLLER_H_ */
