/*
 * RCControllerTest.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#include <linux/joystick.h>

#include <gtest/gtest.h>

#include "RCControllerFixture.h"

namespace{

TEST_F(RCControllerFixture, turnRightTest){
	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 0;
	test_js_event.value = INT16_MAX;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_NEAR(controller_test_max_servopw, test_car->getServoPW(), 0.1);

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 0;
	test_js_event.value = 0;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_NEAR((controller_test_max_servopw + controller_test_min_servopw) / 2, test_car->getServoPW(), 0.1);

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 0;
	test_js_event.value = INT16_MIN;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_NEAR(controller_test_min_servopw, test_car->getServoPW(), 0.1);
}

TEST_F(RCControllerFixture, goForwardTest){
	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 5;
	test_js_event.value = INT16_MAX;//The xbox controller uses signed shorts for triggers so we will use them in this test as we would in the actuall code

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_EQ(controller_test_max_speed, test_car->getSpeed());

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 5;
	test_js_event.value = 0;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_NEAR((controller_test_max_speed / 2), test_car->getSpeed(), 1);

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 5;
	test_js_event.value = INT16_MIN;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_EQ(0, test_car->getSpeed());
	ASSERT_TRUE(test_car->getGoingForward());
}

TEST_F(RCControllerFixture, goBackwardTest){
	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 2;
	test_js_event.value = INT16_MAX;//The xbox controller uses signed shorts for triggers so we will use them in this test as we would in the actuall code

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_EQ(controller_test_max_speed, test_car->getSpeed());

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 2;
	test_js_event.value = 0;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_NEAR((controller_test_max_speed / 2), test_car->getSpeed(), 1);

	test_js_event.type = JS_EVENT_AXIS;
	test_js_event.number = 2;
	test_js_event.value = INT16_MIN;

	test_controller->handleJoystickEvent(test_js_event);
	ASSERT_EQ(0, test_car->getSpeed());
	ASSERT_FALSE(test_car->getGoingForward());
}

}//End namespace
