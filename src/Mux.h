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

#define MUX_COUNT 2


class Mux {
public:
	Mux();
	virtual ~Mux();

	void init();

	void update();


	uint16_t *get(uint8_t index);
	void print();
	void print(uint8_t mux_nr);


	void print_bit(uint8_t pin);


private:

	uint16_t mux_data[MUX_COUNT];

	DigitalIn *mux[MUX_COUNT];

	BusOut mux_adr;


};

#endif /* SRC_MUX_H_ */
