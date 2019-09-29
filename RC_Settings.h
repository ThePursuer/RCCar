/*
 * Settings.h
 *
 *  Created on: Sep 24, 2019
 *      Author: thepursuer
 */

#ifndef RC_SETTINGS_H_
#define RC_SETTINGS_H_

/*This file is to be replaced by a settings class in the future*/

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

//Gearing Maximums
#define REVERSE_MAX (MAX_PWM * 0.4)
#define FIRST_MAX (MAX_PWM * 0.25)
#define SECOND_MAX (MAX_PWM * 0.6)
#define THIRD_MAX (MAX_PWM * 0.7)
#define FOURTH_MAX (MAX_PWM * 0.8)
#define FIFTH_MAX (MAX_PWM * 0.9)
#define SIXTH_MAX (MAX_PWM)

//Gearing Minimums
#define REVERSE_MIN 0
#define FIRST_MIN 0
#define SECOND_MIN (MAX_PWM * 0.1)
#define THIRD_MIN (MAX_PWM * 0.2)
#define FOURTH_MIN (MAX_PWM * 0.3)
#define FIFTH_MIN (MAX_PWM * 0.4)
#define SIXTH_MIN (MAX_PWM * 0.5)

//Joystick related
#define JOYSTICK_FILENAME "/dev/input/js0"

//Motor related
#define L298N_HBRIDGE1_PIN 38 //GPIO20
#define L298N_HBRIDGE2_PIN 40 //GPIO21
#define L298N_EN_PIN (PIN_BASE + 1)
#define RPM_DELTA_CLAMP_VALUE (MAX_PWM * 0.015)
#define FULL_BRAKE_DELTA (MAX_PWM * 0.01)
#define MAX_REAL_SPEED MAX_PWM //We do not have data yet so we will use this value

//Tachometer
#define TACHOMETER_PIN 35

//Sounds related
#define IDLE_RPM 800
#define MAX_SIMULATED_RPM 7500

#endif /* RC_SETTINGS_H_ */
