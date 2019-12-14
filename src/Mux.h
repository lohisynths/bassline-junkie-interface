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

#define MUX_COUNT 4

const PinName MUX_ADDR_PINS[4] = {
    PB_5,   // MUX ADDR A
    PB_4,   // MUX ADDR B
    PB_10,  // MUX ADDR C
    PA_8    // MUX ADDR D
};

const PinName MUX_OUTPUT_PINS[MUX_COUNT] = {
    PA_0,   // MUX 0 OUT
    PA_1,   // MUX 1 OUT
    PA_4,   // MUX 2 OUT
    PB_0    // MUX 3 OUT
};

const PinName FAKE_MUX_INPUT_PINS[] = {
    PB_2,
    PB_1,
    PB_15,
    PB_13,
    PC_2,
    PC_3,
    PC_0,
    PC_1,
    PB_14
};


class Mux {
public:

    Mux() : mux_adr(MUX_ADDR_PINS[0], MUX_ADDR_PINS[1], MUX_ADDR_PINS[2], MUX_ADDR_PINS[3]),
            fake_mux_4(
          FAKE_MUX_INPUT_PINS[0], FAKE_MUX_INPUT_PINS[1], FAKE_MUX_INPUT_PINS[2],
          FAKE_MUX_INPUT_PINS[3], FAKE_MUX_INPUT_PINS[4], FAKE_MUX_INPUT_PINS[5],
          FAKE_MUX_INPUT_PINS[6], FAKE_MUX_INPUT_PINS[7], FAKE_MUX_INPUT_PINS[8]) {
    }

	virtual ~Mux() {};

    /*! \var const char *name
     *  \brief const char * defining class name
     */
    const char *name = "Mux";
    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MUX_LOG_LEVEL> LOG;

	void init();

	void update();

	uint16_t *get(uint8_t index);
	void print();
	void print(uint8_t mux_nr);

	void print_bit(uint8_t mux_index, uint8_t pin);

private:
	uint16_t mux_data[MUX_COUNT+1];
	DigitalIn *mux[MUX_COUNT];
	BusOut mux_adr;
	BusIn fake_mux_4;
};

#endif /* SRC_MUX_H_ */
