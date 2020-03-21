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

#define TRIGGER_DEADZONE 50

#define CHECK_T_ON(val) (val > INT16_MIN + TRIGGER_DEADZONE)
#define BOTH_T_ON(t1, t2) (CHECK_T_ON(t1) && CHECK_T_ON(t2))

struct Controller{
	int16_t r_trig, l_trig;//Triggers
	bool r_bump, l_bump;//Bumpers
	//Left stick
	int16_t lx, ly;
	bool lb;
	//Right stick
	int16_t rx, ry;
	bool rb;
	//Buttons
	bool a, b, x, y, start, select;
};

/**
 * \class RC_Controller
 * \brief Con
 */
class RC_Controller {
public:
	RC_Controller(std::shared_ptr<RC_Car> rc);

	void handleJoystickEvent(js_event event);
private:
	void updateCar();

	std::shared_ptr<RC_Car> rc_;
	Controller controller_;
};

#endif /* RCCONTROLLER_H_ */
