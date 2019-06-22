/*
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include "RCCar.h"

#include <iostream>
#include <unistd.h>

#include <pca9685.h>
#include <wiringPi.h>

using namespace std;

#define PIN_BASE 300
#define SERVO_PIN PIN_BASE
#define MOTOR_PIN (PIN_BASE + 1)
#define MAX_PWM 4096
#define HERTZ 330

#define SERVO_MAX_PW 2.5
#define SERVO_MIN_PW 0.5

int pca9685FD;

/*
 * Utility function to calculate the ticks needed to produce a pulse of impulseMS milliseconds on the pca9685
 */
int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

class MyRCCar: public RCCar{
public:
	MyRCCar(){
		setServoMaxPw(SERVO_MAX_PW);
		setServoMinPw(SERVO_MIN_PW);
	}

protected:
	void update(){
		std::cout << servoPw_ << std::endl;
		pwmWrite(SERVO_PIN, calcTicks(servoPw_, HERTZ));
	}
};

int main(int argc, char **argv) {

	/***************************
	 * Setup the pca9685 first *
	 ***************************/

	// Calling wiringPi setup first.
	wiringPiSetup();

	// Setup with pinbase 300 and i2c location 0x40
	pca9685FD = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (pca9685FD < 0)
	{
		printf("Error in setup\n");
		return pca9685FD;
	}

	// Reset all output
	pca9685PWMReset(pca9685FD);

	/********************
	 * Set up the RCCar *
	 ********************/

	MyRCCar myCar;
	myCar.setUpdateCycle(50);
	myCar.engineOn();

	for(int i = 0; i < 10; i++){
		if(i % 2 == 0)
			myCar.turn(INT16_MIN);
		else
			myCar.turn(INT16_MAX);
		sleep(1);
	}

	return 0;
}


