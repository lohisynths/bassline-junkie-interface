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

Mux::Mux() : mux_adr(PB_5, PB_4, PB_10, PA_8) {

}

static PinName mux_addr[MUX_COUNT] = {PA_0, PA_1};


void Mux::init() {

	for(int i=0; i < MUX_COUNT; i++) {
		mux[i] = new DigitalIn(mux_addr[i]);
	}

}



Mux::~Mux() {


}


void Mux::update() {

	for (uint8_t i = 0; i < 16; i++) {
		mux_adr.write(i);

		for(uint8_t mux_nr=0; mux_nr<MUX_COUNT; mux_nr++) {
			bool bit = mux[mux_nr]->read();
			if (bit)
				SETBIT(mux_data[mux_nr], (i));
			else
				CLEARBIT(mux_data[mux_nr], (i));
		}
	}
}

uint16_t *Mux::get(uint8_t index) {
	return &mux_data[index];
}

void Mux::print_bit(uint8_t pin) {

	for(uint8_t mux_nr=0; mux_nr<MUX_COUNT; mux_nr++) {
		printf("mux %d %u\r\n", mux_nr, CHECKBIT(mux_data[mux_nr], 15-pin));
	}


}

void Mux::print(uint8_t mux_nr) {
	printf("mux %d ", mux_nr);
	for (uint8_t i = 0; i < 16; i++) {
		printf("%u", CHECKBIT(mux_data[mux_nr], 15-i));
		//printf(" ");
	}
	printf("\r\n");
}


void Mux::print() {
	for(uint8_t mux_nr=0; mux_nr<MUX_COUNT; mux_nr++) {
		printf("mux %d ", mux_nr);
		for (uint8_t i = 0; i < 16; i++) {
			printf("%u", CHECKBIT(mux_data[mux_nr], 15-i));
			//printf(" ");
		}
		printf("\r\n");
	}
}
