/*
 * RCClassTest.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */
#include "RCCarFixture.h"
#include <unistd.h>

#include <gtest/gtest.h>
#include "../RC_Settings.h"

namespace{

TEST_F(RCCarFixture, turningTest){
	test_car.turn(0);
	EXPECT_NEAR(((rc_test_min_servopw + rc_test_max_servopw) / 2), test_car.getServoPW(), 0.1);

	test_car.turn(INT16_MAX);
	EXPECT_NEAR(rc_test_max_servopw, test_car.getServoPW(), 0.1);

	test_car.turn(INT16_MIN);
	EXPECT_NEAR(rc_test_min_servopw, test_car.getServoPW(), 0.1);
}

TEST_F(RCCarFixture, accelerateTest){
	test_car.throttle(INT16_MAX);
	ASSERT_EQ(rc_test_max_speed, test_car.getSpeed());

	test_car.throttle(0);
	ASSERT_EQ(rc_test_max_speed / 2, test_car.getSpeed());

	test_car.throttle(INT16_MIN);
	ASSERT_EQ(0, test_car.getSpeed());

	test_car.throttle(INT16_MAX);
	ASSERT_EQ(rc_test_max_speed, test_car.getSpeed());
}

TEST_F(RCCarFixture, brakeTest){
	test_car.brake(INT16_MAX);
	ASSERT_FLOAT_EQ(1.0, test_car.getBrakeForce());

	test_car.brake(0);
	ASSERT_NEAR(0.5, test_car.getBrakeForce(), 0.01);

	test_car.brake(INT16_MIN);
	ASSERT_FLOAT_EQ(0.0, test_car.getBrakeForce());

	test_car.brake(INT16_MAX);
	ASSERT_FLOAT_EQ(1.0, test_car.getBrakeForce());
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

TEST_F(RCCarFixture, gearBoxTest){
	ASSERT_EQ(0, test_car.getGear());

	test_car.gearUp();
	ASSERT_EQ(1, test_car.getGear());

	for(int i = 0; i < 10; i++)
		test_car.gearUp();
	ASSERT_EQ(6, test_car.getGear());

	test_car.gearDown();
	ASSERT_EQ(5, test_car.getGear());

	for(int i = 0; i < 10; i++)
		test_car.gearDown();
	ASSERT_EQ(-1, test_car.getGear());
}

}


