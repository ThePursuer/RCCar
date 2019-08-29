/*
 * RCCarImplementation.h
 *
 *  Created on: Jun 30, 2019
 *      Author: thepursuer
 */

#ifndef TEST_RCCARIMPLEMENTATION_H_
#define TEST_RCCARIMPLEMENTATION_H_
#include "RCCar.h"

class TestRCCar : public RC_Car{
public:
	float getServoPW() const {return servoPw_;}
	uint8_t getSpeed() const {return targetSpeed_;}
	bool getGoingForward() const {return goingForward_;}
	int getUpdateCalledTimes() const {return updateCalled;}
	int getGear() const { return gear_; }

	void setPWForTesting(float min, float max){
		setServoMaxPw(max);
		setServoMinPw(min);
	}

	void update() override {++updateCalled;}//todo mock this function when I can actually read the gmock documentation
private:
	int updateCalled = 0;
};


#endif /* TEST_RCCARIMPLEMENTATION_H_ */
