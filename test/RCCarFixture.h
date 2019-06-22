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

class TestRCCar : public RCCar{
public:
	float getServoPW() const {return servoPw_;}
	uint8_t getSpeed() const {return speed_;}
	bool getGoingForward() const {return goingForward_;}
	int getUpdateCalledTimes() const {return updateCalled;}

	void setPWForTesting(float min, float max){
		setServoMaxPw(max);
		setServoMinPw(min);
	}

	void update() override {++updateCalled;}//todo mock this function when I get out of China and can actually read the gmock documentation
private:
	int updateCalled = 0;
};

class RCCarFixture : public ::testing::Test {
protected:
	virtual void SetUp(){}
	virtual void TearDown(){}
	TestRCCar test_car;
};

#endif /* TEST_RCCARFIXTURE_H_ */
