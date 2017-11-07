/*
 * Enc.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ENC_H_
#define SRC_ENC_H_

#include "mbed.h"

class Enc {
public:
	Enc();
	virtual ~Enc();

	bool update(bool curModeA, bool curModeB	);
	int16_t get();
	void set(int16_t val);


private:
	//////////////// Prepare some global variables
	// number of encoders
	// last mode of each pin (HIGH/LOW) for comparison - see if it changed
	bool lastModeA;
	bool lastModeB;
	// current mode of each encoder pin (HIGH/LOW)

	// current and last encoder positions
	int16_t encPos;
	int16_t encPosLast;

};

#endif /* SRC_ENC_H_ */
