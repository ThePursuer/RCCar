/*
 * RCController.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#include "RCController.h"
#include <string.h>
#include <iostream>

RC_Controller::RC_Controller(std::shared_ptr<RC_Car> rc):
	rc_(rc)
{
	controller_.l_trig = INT16_MIN;
	controller_.r_trig = INT16_MIN;
}

void RC_Controller::handleJoystickEvent(js_event event){
    switch (event.type & ~JS_EVENT_INIT) {
        case JS_EVENT_AXIS:
        	switch (event.number){
        	case 0://Left Joy X-axis, DEJA VU!
        		controller_.lx = event.value;
        		break;
        	case 5://Right Trigger, DO YOU LIKE... MY CAR?
        		controller_.r_trig = event.value;
        		break;
        	case 2://Left Trigger, nobody writes songs about being on the brakes so check it last.
        		controller_.l_trig = event.value;
        		break;
        	default:
        		break;
        	}
            break;
        case JS_EVENT_BUTTON://No use for buttons yet
            break;
        default:
            break;
    }
    updateCar();
}

void RC_Controller::updateCar() {
	rc_->turn(controller_.lx);

	if(BOTH_T_ON(controller_.l_trig, controller_.r_trig))
		rc_->stop();
	else if(CHECK_T_ON(controller_.r_trig)){
		rc_->direction(FORWARD);
		rc_->throttle(controller_.r_trig);
	}
	else if(CHECK_T_ON(controller_.l_trig)){
		rc_->direction(BACKWARD);
		rc_->throttle(controller_.l_trig);
	}
	else
		rc_->stop();
}
