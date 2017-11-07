/*
 * Knob.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_KNOB_H_
#define SRC_KNOB_H_

#include "mbed.h"
#include "Pwm.h"
#include "Enc.h"


struct knob_msg{
	bool value_changed=false;
	bool switch_changed = false;
};

class Knob {
public:
	Knob();
	virtual ~Knob();
	knob_msg update(int32_t mux_data, int32_t sw_data);
	void init(size_t index, Pwm *pwm, int32_t mux_data)	;

	void reset(int16_t mux_data);


	void led_on(size_t led_nr, int16_t bright);

	int16_t get_value();
	void set_value(int16_t val);

	bool get_sw_state();


private:
	Enc encoder;

	Pwm *leds = nullptr;

	uint16_t encoder_value = 0;

	int16_t enc_last = 0;

	uint16_t led_last = 0;

	uint16_t last_sw = 1;


	/////////////////////
	// mux and pwm registers bit numbers
	uint8_t enc_a_bit = 0;
	uint8_t enc_b_bit = 0;
	uint8_t sw_bit = 0;
	uint8_t first_led = 0;



};

#endif /* SRC_KNOB_H_ */
