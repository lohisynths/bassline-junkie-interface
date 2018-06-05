/*
 * Enc.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ENC_H_
#define SRC_ENC_H_

#include "mbed.h"
#include "utils.h"

class Enc {
public:
	Enc();
	virtual ~Enc();

	bool update();
	int16_t get();
	void set(int16_t val);
	void init(uint16_t &data, uint8_t first_bit);




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
	bool curModeA;
	bool curModeB;

	uint16_t *mux_data = nullptr;
	uint8_t bit_a;


};

#endif /* SRC_ENC_H_ */
