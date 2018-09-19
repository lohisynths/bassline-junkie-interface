/*! \file Button.cpp
 *  \brief Source file for Button class.
 *    Main interface for reading switch data and controlling it's illumination.
 *  \author lohi-synths
 *  \version 0.0
 *  \date Nov 1 2017
 */

#include "Button.h"
#include <string>

void Button::init(button_init_map button_configuration) {
	memcpy(&button_config, &button_configuration, sizeof(button_configuration));
	print_config(button_config);
}

bool Button::update() {
	bool ret = false;
	bool sw = CHECKBIT(*button_config.mux_raw_data, button_config.mux_first_bit);
	if (last_state != sw) {
		ret = true;
		last_state = sw;
	}
	return ret;
}

bool Button::get_state() {
	return last_state;
}

void Button::set_led_val(uint16_t brightness) {
	button_config.leds->set(button_config.first_pwm_output, brightness);
}

void Button::print_config(button_init_map config) {
    // uncomment '#define DEBUG_KNOB in Knob.h' for debug prints
#ifdef DEBUG_BUTTON
	std::string sep("\r\n");
	std::string out(sep + std::string("button config: ") + sep +
			"mux                 " + std::to_string((uint32_t)config.mux) + sep +
			"mux_raw_data        " + std::to_string((uint32_t)config.mux_raw_data) + sep +
			"mux_first_bit       " + std::to_string(config.mux_first_bit) + sep +
			"leds                " + std::to_string((uint32_t)config.leds) + sep +
			"max_led_value       " + std::to_string(config.max_led_value) + sep +
			"first_pwm_output    " + std::to_string(config.first_pwm_output) + sep);
	DEBUG_LOG("%s\r\n", out.c_str());
#endif
}
