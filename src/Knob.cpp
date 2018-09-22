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

	// knob_config.mux_first_bit corresponds to rotary encoder rotary switch
	// so first_bit is knob_config.mux_first_bit+1
	uint8_t first_bit = knob_config.mux_first_bit+1;
	encoder.init(*knob_config.mux_raw_data, first_bit);
	// TODO: propper reseting
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
        LOG::LOG0("%s switch state changed changed, ", name);
        LOG::LOG0((sw_state) ? "pushed\r\n" : " released\r\n" );
		ret.switch_changed = true;
		last_sw_state = sw_state;
	}

	bool encoder_value_changed = encoder.update();
	int16_t enc_val  = encoder.get();
	if (encoder_value_changed) {
		// TODO: check if 16 bit if fine
		if (last_enc_value > enc_val && actual_value != 0) {
			actual_value--;
			ret.value_changed=true;
	        LOG::LOG0("%s actual_value %d\r\n", name, actual_value);
		}
		if (last_enc_value < enc_val && actual_value < knob_config.encoder_max_value) {
			actual_value++;
			ret.value_changed=true;
	        LOG::LOG0("%s actual_value %d\r\n", name, actual_value);
		}
	}
	last_enc_value = enc_val;
	return ret;
}

void Knob::set_value(uint16_t value, bool force_led_update){
	actual_value = value / encoder_2_midi_mult;
}

uint16_t Knob::get_knob_value(){
	uint8_t actual_value_scaled = actual_value * encoder_2_midi_mult;
	LOG::LOG1("%s actual_value_scaled %d\r\n", name, actual_value_scaled);
	return actual_value_scaled;
}

bool Knob::get_switch_state(){
	return last_sw_state;
}

uint8_t Knob::calculate_led_position(uint8_t led_number) {
	// leds in PCA9685 registry are located counter-clockwise
	uint8_t led_nr = (knob_config.total_led_count - 1) - led_number;
	led_nr += knob_config.first_pwm_output;
	return led_nr;
}

void Knob::led_on_last_off(uint8_t led_on_nr, uint8_t led_off_nr, int16_t bright) {
	uint8_t led_nr_tmp = calculate_led_position(led_on_nr);
	uint8_t last_led_nr_tmp = calculate_led_position(led_off_nr);

	knob_config.leds->set(last_led_nr_tmp, 0);
	knob_config.leds->set(led_nr_tmp, bright);
}

void Knob::led_indicator_set_value(uint16_t value, bool force) {
	int led_bright = knob_config.max_led_value;
	int led_nr = value * encoder_max_to_127_divider;
	if(led_nr != last_led_on || force) {
	    LOG::LOG2("%s led indication scale value %d\r\n", name, value);
		led_on_last_off(led_nr, last_led_on, led_bright);
		last_led_on = led_nr;
	}
}

void Knob::print_config(knob_init_map config) {
	std::string sep("\r\n");
	std::string out(std::string(std::string(name) + " config: ") + sep +
	        name + " mux                 " + std::to_string((uint32_t)config.mux) + sep +
	        name + " mux_raw_data        " + std::to_string((uint32_t)config.mux_raw_data) + sep +
	        name + " mux_first_bit       " + std::to_string(config.mux_first_bit) + sep +
	        name + " encoder_max_value   " + std::to_string(config.encoder_max_value) + sep +
	        name + " leds                " + std::to_string((uint32_t)config.leds) + sep +
	        name + " max_led_value       " + std::to_string(config.max_led_value) + sep +
	        name + " first_pwm_output    " + std::to_string(config.first_pwm_output) + sep +
	        name + " total_led_count     " + std::to_string(config.total_led_count) + sep);
	LOG::LOG3("%s", out.c_str());
}
