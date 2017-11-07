/*
 * ENC8.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: alax
 */

#include "ENC8.h"

ENC8::ENC8() : enc8(PA_4, PB_0, PC_1) {

/// TODO: replaced in future hw revision
}

ENC8::~ENC8() {
}


void ENC8::update(uint32_t &mux_data, uint32_t &sw_data)
{

	auto data = enc8.read();

	bool enc8_sw = CHECKBIT(data, 0);
	bool enc8_A = CHECKBIT(data, 1);
	bool enc8_B = CHECKBIT(data, 2);


	//printf("%d %d %d\n", enc8_sw, enc8_A, enc8_B);

	if (enc8_sw)
		SETBIT(sw_data, 16);
	else
		CLEARBIT(sw_data, 16);

	if (enc8_A)
		SETBIT(mux_data, 16);
	else
		CLEARBIT(mux_data, 16);

	if (enc8_B)
		SETBIT(mux_data, 17);
	else
		CLEARBIT(mux_data, 17);



}
