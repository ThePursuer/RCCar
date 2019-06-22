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

/*
 * Abstract class which manages an RCCar. Needs to define update() in order to work properly.
 */
class RCCar {
public:
	RCCar();
	virtual ~RCCar();

	void engineOn();
	void EngineOff();//Use this to stop the motor in case of emergency
	bool setUpdateCycle(uint16_t milliseconds);//How long between update() calls

	void turn(int16_t val);//accepts all values of unsigned short
	void forward(uint8_t val);
	void backward(uint8_t val);
	void stop();

protected:
	virtual void update() = 0;//Hardware specific

	volatile float servoPw_;
	volatile uint8_t speed_;
	volatile bool goingForward_;

	void setServoMaxPw(float servoMaxPw = 2.0);
	void setServoMinPw(float servoMinPw = 1.0);

private:
	void loop();

	volatile bool engineIsOn_;
	uint16_t updateCycle_;
	std::thread *updateThread_;

	float servoMinPW = 1.0;
	float servoMaxPW = 2.0;
};

#endif /* RCCAR_H_ */
