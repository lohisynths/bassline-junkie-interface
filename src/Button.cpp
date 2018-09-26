/*! \file Button.cpp
 *  \brief Source file for Button class.
 *   Main class for reading switch state and illumination control.
 *  \author lohi-synths
 *  \version 0.0
 *  \date Nov 1 2017
 */

#include "Button.h"
#include <string>

void Button::init(button_init_map button_configuration, Pwm *_leds, Mux *_mux){
    leds = _leds;
    mux = _mux;
	memcpy(&button_config, &button_configuration, sizeof(button_configuration));
	print_config(button_config);
}

bool Button::update() {
	bool ret = false;
	uint16_t data = *mux->get(button_config.mux_number);
	bool sw = CHECKBIT(data, button_config.mux_first_bit);
	if (last_state != sw) {
        LOG::LOG0("%s", name);
	    LOG::LOG0((last_state) ? "pushed\r\n" : "released\r\n");
		ret = true;
		last_state = sw;
	}
	return ret;
}

bool Button::get_state() {
	return last_state;
}

void Button::set_led_val(uint16_t brightness) {
	leds->set(button_config.first_pwm_output, brightness);
}

void Button::print_config(button_init_map config) {
	std::string sep("\r\n");
	std::string out(std::string(name) + " config: " + sep +
	        "\t\tmux_first_bit       " + std::to_string(config.mux_first_bit) + sep +
	        "\t\tmax_led_value       " + std::to_string(config.max_led_value) + sep +
	        "\t\tfirst_pwm_output    " + std::to_string(config.first_pwm_output) + sep);
	LOG::LOG1("%s", out.c_str());
}
