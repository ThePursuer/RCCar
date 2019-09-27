/*
 * GearBox.cpp
 *
 *  Created on: Sep 23, 2019
 *      Author: thepursuer
 */

#include "GearBox.h"
#include "Settings.h"

GearBox::GearBox(){
	gear_ = 0;

	gears_.push_back(Gear(REVERSE_MIN, REVERSE_MAX));
	gears_.push_back(Gear(0, MAX_PWM));
	gears_.push_back(Gear(FIRST_MIN, FIRST_MAX));
	gears_.push_back(Gear(SECOND_MIN, SECOND_MAX));
	gears_.push_back(Gear(THIRD_MIN, THIRD_MAX));
	gears_.push_back(Gear(FOURTH_MIN, FOURTH_MAX));
	gears_.push_back(Gear(FIFTH_MIN, FIFTH_MAX));
	gears_.push_back(Gear(SIXTH_MIN, SIXTH_MAX));
}

GearBox::~GearBox() {
	// TODO Auto-generated destructor stub
}

void GearBox::gearUp() {
	if (gear_ < 6)
		++gear_;
}

void GearBox::gearDown() {
	if(gear_ > -1)
		--gear_;
}

int GearBox::getGear() const {
	return gear_.load();
}

int GearBox::getGearMax() const {
	return gears_[gear_ + 1].max_speed_;
}

int GearBox::getGearMin() const {
	return gears_[gear_ + 1].min_speed_;
}

//The following is for the gear class within the GearBox
GearBox::Gear::Gear(int min_speed, int max_speed, float ratio):
	min_speed_(min_speed),
	max_speed_(max_speed),
	ratio_(ratio){
}
