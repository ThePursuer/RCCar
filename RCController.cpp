/*
 * RCController.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#include "RCController.h"
#include <string.h>

RC_Controller::RC_Controller(std::shared_ptr<RC_Car> rc):
	rc_(rc)
{
}

void RC_Controller::handleJoystickEvent(js_event event){
    switch (event.type & ~JS_EVENT_INIT) {
        case JS_EVENT_AXIS:
        	switch (event.number){
        	case 0://Left Joy X-axis
        		rc_->turn(event.value);
        		//js_.joy_left_x = event.value;
        		break;
        	case 5://Right Trigger
        		rc_->throttle(event.value);
        		//js_.rightTrigger = event.value;
        		break;
        	case 2://Left Trigger
        		rc_->brake(event.value);
        		//js_.leftTrigger = event.value;
        		break;
        	default:
        		break;
        	}
            break;
        case JS_EVENT_BUTTON://No use for buttons yet
        	if(event.number == 4){
        		if(event.value && !js_.leftBumper)
        			rc_->gearDown();
        		js_.leftBumper = event.value;
        	}
        	else if(event.number == 5){
        		if(event.value && !js_.rightBumper)
        			rc_->gearUp();
        		js_.rightBumper = event.value;
        	}
            break;
        default:
            break;
    }
}
