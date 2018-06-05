/*
 * Mux.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Mux.h"


// PA_0 - mux 0 - env encoders, env buttons
// 0-2  - env encoder 0
// 3-5  - env encoder 1
// 6-8  - env encoder 2
// 9-11 - env encoder 3
// 12   - env button  loop
// 13   - env button  2
// 14   - env button  1
// 15   - env button  0

Mux::Mux() {

}

void Mux::init(PinName input_pin) {
	mux_adr = new BusOut(PB_5, PB_4, PB_10, PA_8);
	mux = new DigitalIn(input_pin);
}



Mux::~Mux() {


}


void Mux::update() {
	for (uint8_t i = 0; i < 16; i++) {
		mux_adr->write(i);
		bool bit = mux->read();
		if (bit)
			SETBIT(mux_data, (i));
		else
			CLEARBIT(mux_data, (i));
	}
}

uint16_t &Mux::get() {
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
