/*
 * Utilities.h
 *
 *  Created on: Sep 27, 2019
 *      Author: thepursuer
 */

#ifndef RC_UTILITIES_H_
#define RC_UTILITIES_H_

template<class T1, class T2>
T2 number_map(T1 x, T1 in_min, T1 in_max, T2 out_min, T2 out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <class T>
T clamp(T val, T minval, T maxval){
	if(val < minval)
		return minval;
	else if( val > maxval)
		return maxval;
	return val;
}

#endif /* RC_UTILITIES_H_ */
