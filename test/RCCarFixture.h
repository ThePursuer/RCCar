/*
 * RCCarFixture.h
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#ifndef TEST_RCCARFIXTURE_H_
#define TEST_RCCARFIXTURE_H_

#include <gtest/gtest.h>
#include "RCCar.h"
#include "RCCarImplementation.h"

class RCCarFixture : public ::testing::Test {
protected:
	virtual void SetUp(){}
	virtual void TearDown(){}
	TestRCCar test_car;

	const int rc_test_max_speed = MAX_PWM;

	const float rc_test_min_servopw = SERVO_MIN_PW;
	const float rc_test_max_servopw = SERVO_MAX_PW;
};

#endif /* TEST_RCCARFIXTURE_H_ */
