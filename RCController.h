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
	struct joystickTriggers js_;
	std::shared_ptr<RC_Car> rc_;
};

#endif /* RCCONTROLLER_H_ */
