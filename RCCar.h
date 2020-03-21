/*
 * RCCar.h
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#ifndef RCCAR_H_
#define RCCAR_H_

#include <stdint.h>
#include <thread>
#include <mutex>
#include "RC_Settings.h"

enum Direction{
	FORWARD,
	BACKWARD
};

/**
 * \interface RC_Car
 * \brief Abstract class which manages an RCCar. Update hardware by implementing update().
 */
class RC_Car {
public:
	RC_Car();
	virtual ~RC_Car();

	void engineOn();
	void EngineOff();//Use this to stop the motor in case of emergency
	bool setUpdateCycle(uint16_t milliseconds);//How long between update() calls

	void turn(int16_t val);//accepts all values of unsigned short
	void throttle(int16_t val);
	void direction(Direction val);

	void stop();
protected:
	virtual void update() = 0;

	volatile float servoPw_;
	volatile int throttle_;
	volatile Direction direction_;
private:
	void loop();

	volatile bool engineIsOn_;
	uint16_t updateCycle_;
	std::thread *updateThread_;

	float servoMinPW_ = SERVO_MIN_PW;
	float servoMaxPW_ = SERVO_MAX_PW;

	int maxSpeed_ = MAX_PWM;
	int minSpeed_ = 0;

	std::mutex updateMu_;
};

#endif /* RCCAR_H_ */
