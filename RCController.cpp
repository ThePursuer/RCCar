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
        		break;
        	case 5://Right Trigger
        		if(js_.leftTrigger > INT16_MIN + 1)//Make sure we arnt holding both triggers (Plus one because the triggers never actuall hit INT16_MIN)
        			rc_->forward(INT16_MIN);
        		else
        			rc_->forward(event.value);

        		js_.rightTrigger = event.value;
        		break;
        	case 2://Left Trigger
        		if(js_.rightTrigger > INT16_MIN + 1)//Make sure we arnt holding both triggers (Plus one because the triggers never actuall hit INT16_MIN)
        			rc_->forward(INT16_MIN);
        		else
        			rc_->backward(event.value);

        		js_.leftTrigger = event.value;
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
}
