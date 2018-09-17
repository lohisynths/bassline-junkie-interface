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
#include "Mux.h"
#include "Enc.h"

class Knob {
public:
	struct knob_msg{
		bool value_changed=false;
		bool switch_changed = false;
	};

	struct knob_init_map {
		Mux *mux;
		uint16_t *mux_raw_data;
		uint8_t mux_first_bit;
		uint16_t knob_max_val;

		Pwm *leds;
		uint8_t max_led_value;
		uint8_t first_pwm_output;
		uint8_t total_led_count;
	};

	Knob();
	virtual ~Knob();

	void init(knob_init_map config);
	knob_msg update();
	void set_leds(uint16_t value);
	void set_value(uint16_t val);
	uint16_t get_knob_value();
	bool get_switch_state();
	knob_init_map knob_config={};

private:
	void led_on_last_off(size_t led_nr, int16_t bright);
	void print_config(knob_init_map config);

	Enc encoder;

	uint16_t encoder_value = 0;
	int16_t encoder_value_last = 0;
	uint16_t led_last = 0;
	uint16_t last_sw_state = 1;

	float divider;
	float val_divider;
};

#endif /* SRC_KNOB_H_ */
