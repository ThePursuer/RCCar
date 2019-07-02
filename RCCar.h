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
class RC_Car {
public:
	RC_Car();
	virtual ~RC_Car();

	void engineOn();
	void EngineOff();//Use this to stop the motor in case of emergency
	bool setUpdateCycle(uint16_t milliseconds);//How long between update() calls

	void turn(int16_t val);//accepts all values of unsigned short
	void forward(int16_t val);
	void backward(int16_t val);
	void stop();

protected:
	virtual void update() = 0;//Hardware specific, needs implementation

	volatile float servoPw_;
	volatile uint8_t speed_;
	volatile bool goingForward_;

	void setServoMaxPw(float servoMaxPw);
	void setServoMinPw(float servoMinPw);

private:
	void loop();

	volatile bool engineIsOn_;
	uint16_t updateCycle_;
	std::thread *updateThread_;

	float servoMinPW = 1.0;
	float servoMaxPW = 2.0;
};

#endif /* RCCAR_H_ */
