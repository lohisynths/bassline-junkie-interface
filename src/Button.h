/*
 * Button.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#define DEBUG_BUTTON

#include "mbed.h"
#include "Pwm.h"
#include "Mux.h"
#include "utils.h"

class Button {
public:

	struct button_init_map {
		Mux *mux;
		uint16_t *mux_raw_data;
		uint8_t mux_first_bit;

		Pwm *leds;
		uint8_t max_led_value;
		uint8_t first_pwm_output;
	};

	Button();
	virtual ~Button();

	bool update();

	bool get();

	void init(button_init_map config);

	void set_led_val(uint16_t val);

	button_init_map button_config;

private:

	uint8_t last_sw=1;

	void print_config(button_init_map config);

};

#endif /* SRC_BUTTON_H_ */
