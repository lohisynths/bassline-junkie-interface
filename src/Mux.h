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
	Mux();
	virtual ~Mux();

	void init(PinName input_pin);

	void update();


	uint16_t &get();
	void print();

	void print_bit(uint8_t pin);


private:
	uint16_t mux_data=0;

	DigitalIn *mux;

	BusOut *mux_adr;


};

#endif /* SRC_MUX_H_ */
