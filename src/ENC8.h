/*
 * ENC8.h
 *
 *  Created on: Nov 7, 2017
 *      Author: alax
 */

#ifndef SRC_ENC8_H_
#define SRC_ENC8_H_
#include "utils.h"
#include "mbed.h"

class ENC8 {
public:
	ENC8();
	virtual ~ENC8();

	void update(uint32_t &mux_data, uint32_t &sw_data);
	BusIn enc8;

};

#endif /* SRC_ENC8_H_ */
