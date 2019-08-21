/*
 * TachometerTest.cpp
 *
 *  Created on: Aug 16, 2019
 *      Author: thepursuer
 */

#include "Tachometer.h"

#include <gtest/gtest.h>

#include <unistd.h>
#include <chrono>

namespace{

using namespace std::chrono;


TEST(TachometerCase, TachometerTimeoutTest){
	Tachometer tach(2000);

	tach.pulse();
	sleep(1);
	tach.pulse();

	EXPECT_NEAR(60, tach.getRPM(), 1);

	sleep(1);
	ASSERT_NE(0, tach.getRPM());
	sleep(2);

	ASSERT_EQ(0, tach.getRPM());
}

TEST(TachometerCase, TachometerRPMTest){
	Tachometer tach(2000);

	tach.pulse();
	sleep(1);
	tach.pulse();

	EXPECT_NEAR(60, tach.getRPM(), 1);


	tach.pulse();
	usleep(500000);
	tach.pulse();

	EXPECT_NEAR(120, tach.getRPM(), 1);

	tach.pulse();
	sleep(2);
	tach.pulse();

	EXPECT_NEAR(30, tach.getRPM(), 1);
}

}
