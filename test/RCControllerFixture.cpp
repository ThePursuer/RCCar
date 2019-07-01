/*
 * RCControllerFixture.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#include "RCControllerFixture.h"

void RCControllerFixture::SetUp() {
	test_car = std::make_shared<TestRCCar>();
	test_controller = std::make_shared<RC_Controller>(test_car);
}
