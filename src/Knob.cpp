/*! \file Knob.cpp
 *  \brief Source file for Knob class.
 *   Main class for reading encoder data and illumination control.
 *  \author lohi-synths
 *  \version 0.0
 *  \date Nov 1 2017
 */

#include "Knob.h"
#include "utils.h"
#include <string>

void Knob::init(knob_init_map config) {
	memcpy(&knob_config, &config, sizeof(knob_init_map));

	encoder.init(*knob_config.mux_raw_data, knob_config.mux_first_bit+1);
	encoder.update();
	encoder.set(0);
	encoder.update();

	encoder_max_to_127_divider = (float)(knob_config.total_led_count-1) / 127.;
	encoder_2_midi_mult = 127. / (float)knob_config.encoder_max_value ;

	print_config(config);
}

Knob::knob_msg Knob::update() {
	knob_msg ret={};
	bool sw_state = CHECKBIT(*knob_config.mux_raw_data, knob_config.mux_first_bit);

	if (last_sw_state != sw_state) {
		ret.switch_changed = true;
		last_sw_state = sw_state;
	}

	bool encoder_value_changed = encoder.update();
	if (encoder_value_changed) {
		int16_t enc = encoder.get();

		if (encoder_value_last > enc) {
			if (encoder_value != 0)
				encoder_value--;
		} else {
			if (encoder_value < knob_config.encoder_max_value)
				encoder_value++;
		}
		encoder_value_last = enc;
		ret.value_changed=true;
	}
	return ret;
}

void Knob::set_value(uint16_t val){
	encoder_value = val / encoder_2_midi_mult;
}

uint16_t Knob::get_knob_value(){
	return encoder_value * encoder_2_midi_mult;
}

bool Knob::get_switch_state(){
	return last_sw_state;
}

void Knob::led_on_last_off(size_t led_nr, int16_t bright) {
	led_nr = (knob_config.total_led_count-1) - led_nr;
	led_nr += knob_config.first_pwm_output;
	knob_config.leds->set(led_last, 0);
	knob_config.leds->set(led_nr, bright);
	led_last = led_nr;
}
void Knob::set_leds(uint16_t value) {
	int led_bright = 256;
	int led_nr = value * encoder_max_to_127_divider;
	led_on_last_off(led_nr, led_bright);
}

void Knob::print_config(knob_init_map config) {
    // uncomment '#define DEBUG_KNOB in Knob.h' for debug prints
#ifdef DEBUG_KNOB
	std::string sep("\r\n");
	std::string out(sep + std::string("knob config: ") + sep +
			"mux                 " + std::to_string((uint32_t)config.mux) + sep +
			"mux_raw_data        " + std::to_string((uint32_t)config.mux_raw_data) + sep +
			"mux_first_bit       " + std::to_string(config.mux_first_bit) + sep +
			"encoder_max_value   " + std::to_string(config.encoder_max_value) + sep +
			"leds                " + std::to_string((uint32_t)config.leds) + sep +
			"max_led_value       " + std::to_string(config.max_led_value) + sep +
			"first_pwm_output    " + std::to_string(config.first_pwm_output) + sep +
			"total_led_count     " + std::to_string(config.total_led_count) + sep);
	DEBUG_LOG("%s\r\n", out.c_str());
#endif
}
