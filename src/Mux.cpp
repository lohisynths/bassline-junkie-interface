/*
 * Mux.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Mux.h"
#include <bitset>

static PinName mux_addr[MUX_COUNT] = {PA_0, PA_1, PA_4, PB_0};

void Mux::init() {
	for(int i=0; i < MUX_COUNT; i++) {
		mux[i] = new DigitalIn(mux_addr[i]);
	}
}

const char *Mux::get_binary(uint16_t input) {
    return std::bitset<16>(input).to_string().c_str();
}

void Mux::update() {
    // without optimizations, with LOG_LEVELS::DISABLED
    // measured time for 4 function calls below = 25 us
    //  vs 4 us without call to get_binary(mux_data[*])
   LOG::LOG0( "%s 0 data %s\r\n", name, "");//get_binary(mux_data[0]));
   LOG::LOG1( "%s 1 data %s\r\n", name, "");//get_binary(mux_data[1]));
   LOG::LOG2( "%s 2 data %s\r\n", name, "");//get_binary(mux_data[2]));
   LOG::LOG3( "%s 3 data %s\r\n", name, "");//get_binary(mux_data[3]));

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
	mux_data[MUX_COUNT] = mux_raw;
}

uint16_t *Mux::get(uint8_t index) {
	return &mux_data[index];
}

void Mux::print_bit(uint8_t mux_index, uint8_t pin) {
    LOG::LOG4("%s %d %u\r\n", name, mux_index, CHECKBIT(mux_data[mux_index], pin));
}

void Mux::print(uint8_t mux_nr) {
    LOG::LOG4("&s %d ", name, mux_nr);
	for (uint8_t i = 0; i < 16; i++) {
	    LOG::LOG4("%s %u", name, CHECKBIT(mux_data[mux_nr], (15-i)));
	}
	LOG::LOG4("\r\n");
}

void Mux::print() {
	for(uint8_t mux_nr=0; mux_nr<MUX_COUNT; mux_nr++) {
	    LOG::LOG4("%s %d ", name, mux_nr);
		for (uint8_t i = 0; i < 16; i++) {
		    LOG::LOG4("%s %u", name, CHECKBIT(mux_data[mux_nr], (15-i)));
		}
		LOG::LOG4("\r\n");
	}
}
