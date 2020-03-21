/*
 * RCCar_imp.h
 *
 *  Created on: Mar 21, 2020
 *      Author: thepursuer
 */

#ifndef RCCAR_IMP_H_
#define RCCAR_IMP_H_

#include <memory>

#include "RCCar.h"
#include "EngineAudio.h"

//Implement an RC Car
class RCCarImp: public RC_Car{
public:
	RCCarImp(std::shared_ptr<EngineAudio> audio);

protected:
	void update();

private:
	int load_;
};



#endif /* RCCAR_IMP_H_ */
