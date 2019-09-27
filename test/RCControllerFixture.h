/*
 * RCControllerFixture.h
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#ifndef TEST_RCCONTROLLERFIXTURE_H_
#define TEST_RCCONTROLLERFIXTURE_H_

#include <memory>

#include <gtest/gtest.h>

#include "RCCar.h"
#include "RCController.h"
#include "RCCarImplementation.h"

class RCControllerFixture : public ::testing::Test {
protected:
	virtual void SetUp();
	virtual void TearDown(){}

	std::shared_ptr<TestRCCar> test_car;
	std::shared_ptr<RC_Controller> test_controller;
	struct js_event test_js_event;

	const int controller_test_max_speed = MAX_PWM;

	const float controller_test_min_servopw = SERVO_MIN_PW;
	const float controller_test_max_servopw = SERVO_MAX_PW;
};

#endif /* TEST_RCCONTROLLERFIXTURE_H_ */
