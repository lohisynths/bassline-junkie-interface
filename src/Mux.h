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


class Mux {
public:
    // PA_0 - mux 0 - env encoders, env buttons
    // 0-2  - env encoder 0
    // 3-5  - env encoder 1
    // 6-8  - env encoder 2
    // 9-11 - env encoder 3
    // 12   - env button  loop
    // 13   - env button  2
    // 14   - env button  1
    // 15   - env button  0
    // PB_2, PB_1, PB_15 - LFO knob
    // PH_1, PC_2, PC_3, PC_0, PC_1, PH_0
    //mux_raw(PB_2, PB_1, PB_15, /**/  PH_1, PC_2, PC_3, PC_0, PC_1, PH_0) {
    //mux_raw(PB_2, PB_1, PB_15, PB_14, PB_13) {
    Mux() : mux_adr(PB_5, PB_4, PB_10, PA_8),
                 mux_raw(PB_2, PB_1, PB_15, /**/  PB_13, PC_2, PC_3, PC_0, PC_1, PB_14) {
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

	const char *get_binary(uint16_t input);

	uint16_t *get(uint8_t index);
	void print();
	void print(uint8_t mux_nr);


	void print_bit(uint8_t mux_index, uint8_t pin);


private:
	uint16_t mux_data[MUX_COUNT+1];
	DigitalIn *mux[MUX_COUNT];
	BusOut mux_adr;
	BusIn mux_raw;
};

#endif /* SRC_MUX_H_ */
