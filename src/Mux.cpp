/*
 * Mux.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Mux.h"

//	DigitalIn mux1;//(PA_0);
//	DigitalIn mux2;//(PA_1);
//
//	BusOut mux_adr;//(PA_8, PB_10, PB_4, PB_5);

static PinName inputs[2]={
	PA_0,
	PA_1
};

Mux::Mux(uint8_t input_pin) :
		mux_data(0), mux(inputs[input_pin]), mux_adr(PA_8, PB_10, PB_4, PB_5) {

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

uint32_t Mux::get() {
	update();
	return mux_data;
}

void Mux::print() {
	for (uint8_t i = 0; i < 32; i++) {
		printf("%lu", CHECKBIT(mux_data, i));
		printf(" ");
	}
	printf("\n");

}
