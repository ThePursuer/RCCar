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
};

#endif /* TEST_RCCONTROLLERFIXTURE_H_ */
