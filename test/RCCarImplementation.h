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
	int getSpeed() const {return throttle_;}
	int getUpdateCalledTimes() const {return updateCalled;}
	Direction getDirection() const {return direction_;}

	void update() override {++updateCalled;}//todo mock this function when I can actually read the gmock documentation
private:
	int updateCalled = 0;
};


#endif /* TEST_RCCARIMPLEMENTATION_H_ */
