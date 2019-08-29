/*
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include <iostream>
#include <functional>
#include <algorithm>

#include <pca9685.h>
#include <wiringPi.h>

#include <unistd.h>
#include <sys/stat.h>
#include <linux/joystick.h>
#include <fcntl.h>

#include "RCCar.h"
#include "RCController.h"
#include "Tachometer.h"

using namespace std;

//Main related
bool run = true;

//Utility
#define ERROR(msg) do {cout << "ERROR: " << msg << endl;} while(0)
#define INFO(msg) do {cout << "INFO: " << msg << endl;} while(0)

//PCA9685 related
#define PIN_BASE 300
#define SERVO_PIN PIN_BASE
#define MAX_PWM 4096
#define HERTZ 330

#define SERVO_MAX_PW 1.7
#define SERVO_MIN_PW 0.5

int pca9685FD;

//Joystick related
#define JOYSTICK_FILENAME "/dev/input/js0"
int joystickfd = -1;

//Motor related
#define L298N_HBRIDGE1_PIN 38 //GPIO20
#define L298N_HBRIDGE2_PIN 40 //GPIO21
#define L298N_EN_PIN (PIN_BASE + 1)

//Tachometer
#define TACHOMETER_PIN 35
#define EST_MAX_RPM 1300
std::function<void(void)> tachometerCallback;
void tachometerCallback_wrapper(){
	//We cannot pass the Tachomter function directly to wiring pi because of "ruuuules, shhhlerp <pushes up his glasses>"..,
	//so we will wrap it in this function and pass the wrapper insted
	//Screw the rules, I have money!
	tachometerCallback();
}

//Gearing Maximums
#define REVERSE_MAX (MAX_PWM * 0.3)
#define FIRST_MAX (MAX_PWM * 0.2)
#define SECOND_MAX (MAX_PWM * 0.3)
#define THIRD_MAX (MAX_PWM * 0.45)
#define FOURTH_MAX (MAX_PWM * 0.6)
#define FIFTH_MAX (MAX_PWM * 0.8)
#define SIXTH_MAX (MAX_PWM)

//Gearing Minimums
#define REVERSE_MIN 0
#define FIRST_MIN 0
#define SECOND_MIN (MAX_PWM * 0.1)
#define THIRD_MIN (MAX_PWM * 0.2)
#define FOURTH_MIN (MAX_PWM * 0.3)
#define FIFTH_MIN (MAX_PWM * 0.4)
#define SIXTH_MIN (MAX_PWM * 0.5)

/*
 * Utility function to calculate the ticks needed to produce a pulse of impulseMS milliseconds on the pca9685
 */
int calcTicks(float impulseMs)
{
	float cycleMs = 1000.0f / HERTZ;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

int intMap(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
 * Implement an RC Car
 */
class MyRCCar: public RC_Car{
public:
	MyRCCar(shared_ptr<Tachometer> tach):
		tach_(tach),
		realSpeed_(),
		engineSpeed_(),
		previousGear_(){
		setServoMaxPw(SERVO_MAX_PW);
		setServoMinPw(SERVO_MIN_PW);

		setMaxSpeed(MAX_PWM);
	}

	void updateSpeed(int rpm){ realSpeed_ = intMap(rpm, 0, EST_MAX_RPM, 0, MAX_PWM); }

protected:

	void update(){
		//Update speed from tachometer
		updateSpeed(tach_->getRPM());

		//Update steering
		pwmWrite(SERVO_PIN, calcTicks(servoPw_));

		if(gear_ > -1) {
			digitalWrite(L298N_HBRIDGE1_PIN, HIGH);
			digitalWrite(L298N_HBRIDGE2_PIN, LOW);
		}
		else {
			digitalWrite(L298N_HBRIDGE1_PIN, LOW);
			digitalWrite(L298N_HBRIDGE2_PIN, HIGH);
		}

		if(targetSpeed_ > engineSpeed_ )
			engineSpeed_ += (targetSpeed_ - engineSpeed_) * 0.05;
		else
			engineSpeed_ += (targetSpeed_ - engineSpeed_) * 0.08;

		switch (gear_){
		case -1:
			engineSpeed_ = std::min<int>(engineSpeed_, REVERSE_MAX);
			break;
		case 0:
			engineSpeed_ = 0;
			break;
		case 1:
			engineSpeed_ = std::min<int>(engineSpeed_, FIRST_MAX);
			break;
		case 2:
			if(engineSpeed_ < SECOND_MIN)
				gear_--;
			else
				engineSpeed_ = std::min<int>(engineSpeed_, SECOND_MAX);
			break;
		case 3:
			if(engineSpeed_ < THIRD_MIN)
				gear_--;
			else
				engineSpeed_ = std::min<int>(engineSpeed_, THIRD_MAX);
			break;
		case 4:
			if(engineSpeed_ < FOURTH_MIN)
				gear_--;
			else
				engineSpeed_ = std::min<int>(engineSpeed_, FOURTH_MAX);
			break;
		case 5:
			if(engineSpeed_ < FIFTH_MIN)
				gear_--;
			else
				engineSpeed_ = std::min<int>(engineSpeed_, FIFTH_MAX);
			break;
		case 6:
			if(engineSpeed_ < SIXTH_MIN)
				gear_--;
			else
				engineSpeed_ = std::min<int>(engineSpeed_, SIXTH_MAX);
			break;
		default:
			ERROR("GEAR UNRECOGNIZED");
			break;
		}

		pwmWrite(L298N_EN_PIN, engineSpeed_);

		previousGear_ = gear_;
	}
private:
	shared_ptr<Tachometer> tach_;

	int engineSpeed_;
	int realSpeed_;

	int previousGear_;
};

int main(int argc, char **argv) {

	/***************************
	 * Setup the pca9685 first *
	 ***************************/

	// Calling wiringPi setup first.
	wiringPiSetupPhys();

	pinMode(L298N_HBRIDGE1_PIN, OUTPUT);
	pinMode(L298N_HBRIDGE2_PIN, OUTPUT);

	// Setup with pinbase 300 and i2c location 0x40
	pca9685FD = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (pca9685FD < 0)
	{
		ERROR("Error in setup\n");
		return pca9685FD;
	}

	// Reset all output
	pca9685PWMReset(pca9685FD);

	/**************************
	 * Open the Joystick file *
	 **************************/

	joystickfd = open(JOYSTICK_FILENAME, O_RDONLY);
	while(joystickfd < 0)
	{
		ERROR("Could not open joystick file... Retrying");
		joystickfd = open(JOYSTICK_FILENAME, O_RDONLY);

		sleep(2);
	}

	/************************************
	 * Set up the RC_Car and Controller *
	 ************************************/

	//Start with the tachometer
	shared_ptr<Tachometer> tach = make_shared<Tachometer>(1000);
	pinMode(TACHOMETER_PIN, INPUT);
	//Set the callback and pass the wrapper function
	tachometerCallback = bind(&Tachometer::pulse, tach);
	wiringPiISR(TACHOMETER_PIN, INT_EDGE_FALLING, tachometerCallback_wrapper);

	shared_ptr<MyRCCar> myCar = make_shared<MyRCCar>(tach);
	myCar->setUpdateCycle(50);
	myCar->engineOn();

	RC_Controller controller(myCar);

	/***********************
	 * Start the main loop *
	 ***********************/

	struct js_event jsEvent;
	while(run){
		while(read(joystickfd, &jsEvent, sizeof(jsEvent)))
			controller.handleJoystickEvent(jsEvent);
	}

	/***********
	 * Cleanup *
	 ***********/

	close(joystickfd);
	return 0;
}


