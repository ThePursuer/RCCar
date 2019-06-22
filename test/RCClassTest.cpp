/*
 * RCClassTest.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */
#include "RCCarFixture.h"

#include <unistd.h>

#include <gtest/gtest.h>

namespace{

TEST_F(RCCarFixture, turningTest){
	test_car.turn(0);
	EXPECT_NEAR(1.5, test_car.getServoPW(), 0.1);

	test_car.turn(INT16_MAX);
	EXPECT_NEAR(2.0, test_car.getServoPW(), 0.1);

	test_car.turn(INT16_MIN);
	EXPECT_NEAR(1.0, test_car.getServoPW(), 0.1);
}

TEST_F(RCCarFixture, setMinMaxServoPulseWidth){
	test_car.setPWForTesting(0.5, 2.5);

	test_car.turn(INT16_MAX);
	EXPECT_NEAR(2.5, test_car.getServoPW(), 0.1);

	test_car.turn(INT16_MIN);
	EXPECT_NEAR(0.5, test_car.getServoPW(), 0.1);
}

TEST_F(RCCarFixture, accelerateTest){
	test_car.forward(100);
	ASSERT_EQ(100, test_car.getSpeed());
	ASSERT_TRUE(test_car.getGoingForward());

	test_car.backward(100);
	ASSERT_EQ(100, test_car.getSpeed());
	ASSERT_FALSE(test_car.getGoingForward());

	test_car.forward(UINT8_MAX);
	ASSERT_EQ(100, test_car.getSpeed());
	ASSERT_TRUE(test_car.getGoingForward());

	test_car.backward(UINT8_MAX);
	ASSERT_EQ(100, test_car.getSpeed());
	ASSERT_FALSE(test_car.getGoingForward());
}

TEST_F(RCCarFixture, engineOnTest){
	test_car.engineOn();
	usleep(20000);

	EXPECT_TRUE(test_car.getUpdateCalledTimes() > 0);

}

TEST_F(RCCarFixture, setUpdateCyclesTest){
	const int update_cycle = 50;
	const int cycles_to_wait = 10;

	test_car.setUpdateCycle(update_cycle);
	test_car.engineOn();
	usleep(update_cycle * 1000 * cycles_to_wait + 1000);

	EXPECT_NEAR(cycles_to_wait, test_car.getUpdateCalledTimes(), 1);
}

}


