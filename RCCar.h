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
	volatile int speed_;
	volatile bool goingForward_;

	//The below functions set the Min/Max of output values possible for the car.
	//The values should be used to change real values on the hardware.
	//We assume use of a standard RC servo and pwm driven motor drive board
	void setServoMaxPw(float servoMaxPw);
	void setServoMinPw(float servoMinPw);

	void setMaxSpeed(int speed);
	void setMinSpeed(int speed);

private:
	void loop();

	volatile bool engineIsOn_;
	uint16_t updateCycle_;
	std::thread *updateThread_;

	float servoMinPW_ = 1.0;
	float servoMaxPW_ = 2.0;

	int maxSpeed_ = 255;
	int minSpeed_ = 0;
};

#endif /* RCCAR_H_ */
