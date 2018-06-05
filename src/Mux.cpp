/*
 * Mux.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Mux.h"


Mux::Mux(PinName input_pin) :
		mux_data(0), mux(input_pin), mux_adr(PB_5, PB_4, PB_10, PA_8) {

}

Mux::~Mux() {


}


void Mux::update() {
	for (uint8_t i = 0; i < 16; i++) {
		mux_adr.write(i);
		bool bit = mux.read();
		if (bit)
			SETBIT(mux_data, (i));
		else
			CLEARBIT(mux_data, (i));
	}
}

uint16_t Mux::get() {
	update();
	return mux_data;
}

void Mux::print_bit(uint8_t pin) {
	printf("%u\r\n", CHECKBIT(mux_data, 15-pin));

}

void Mux::print() {
	for (uint8_t i = 0; i < 16; i++) {
		printf("%u", CHECKBIT(mux_data, 15-i));
		//printf(" ");
	}
	printf("\r\n");

}
