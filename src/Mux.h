/*
 * Mux.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_MUX_H_
#define SRC_MUX_H_

#include "utils.h"
#include "mbed.h"




class Mux {
public:
	Mux(uint8_t input_pin);
	virtual ~Mux();

	void update();


	uint32_t get();
	void print();


private:
	uint32_t mux_data=0;

	DigitalIn mux;

	BusOut mux_adr;


};

#endif /* SRC_MUX_H_ */
