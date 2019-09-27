/*
 * GearBox.h
 *
 *  Created on: Sep 23, 2019
 *      Author: thepursuer
 */

#ifndef GEARBOX_H_
#define GEARBOX_H_

#include <atomic>
#include <vector>

class GearBox {
public:
	GearBox();
	virtual ~GearBox();

	void gearUp();
	void gearDown();
	int getGear() const;
	int getGearMax() const;
	int getGearMin() const;
private:
	class Gear{
	public:
		Gear(int min_speed, int max_speed, float ratio = 0.0);
		const float ratio_;//This may be used later
		const int min_speed_;
		const int max_speed_;
	};

	std::atomic<int> gear_;
	std::vector<Gear> gears_;
};

#endif /* GEARBOX_H_ */
