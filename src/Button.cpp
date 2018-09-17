/*
 * Button.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Button.h"
#include <string>

struct sw_map {
	uint8_t led;
	uint8_t sw_bit;
};

Button::Button() {

}

Button::~Button() {

}

void Button::init(button_init_map config) {
	memcpy(&button_config, &config, sizeof(config));
	print_config(button_config);
}

bool Button::update() {
	bool ret = false;

	bool sw = CHECKBIT(*button_config.mux_raw_data, button_config.mux_first_bit);

	if (last_sw != sw) {
		ret = true;
		last_sw = sw;
	}

	return ret;
}

bool Button::get()
{
	return last_sw;
}

void Button::set_led_val(uint16_t val)
{
	button_config.leds->set(button_config.first_pwm_output, val);
}

void Button::print_config(button_init_map config) {
    // uncomment '#define DEBUG_KNOB in Knob.h' for debug prints
#ifdef DEBUG_BUTTON
	std::string sep("\r\n");
	std::string out(std::string("config: ") + sep +
			"mux                 " + std::to_string((uint32_t)config.mux) + sep +
			"mux_raw_data        " + std::to_string((uint32_t)config.mux_raw_data) + sep +
			"mux_first_bit       " + std::to_string(config.mux_first_bit) + sep +
			"leds                " + std::to_string((uint32_t)config.leds) + sep +
			"max_led_value       " + std::to_string(config.max_led_value) + sep +
			"first_pwm_output    " + std::to_string(config.first_pwm_output));
	DEBUG_LOG("%s\r\n", out.c_str());
#endif
}
