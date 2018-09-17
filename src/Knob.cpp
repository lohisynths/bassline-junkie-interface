/*
 * Knob.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Knob.h"
#include "utils.h"

Knob::~Knob() {
}
#include <string>


void Knob::init(knob_map config) {
	knob_leds_count = config.led_count;
	knob_max_val = config.knob_max_val;
	leds = config.leds;

	m_mux_index = config.enc_bit;
	m_mux_data = config.mux->get(config.mux_nr);
	m_first_led = config.first_led;

	encoder.init(*m_mux_data, m_mux_index+1);
	encoder.update();
	encoder.set(0);

	divider = (float)(knob_leds_count-1) / 127.;
	val_divider = 127. / (float)knob_max_val ;

	std::string sep(" ");
	std::string out(
			std::to_string(knob_leds_count) + sep +
			std::to_string(knob_max_val) + sep +
			std::to_string((uint32_t)leds) + sep +
			std::to_string((uint32_t)m_mux_data) + sep +
			std::to_string(m_first_led) + sep +
			std::to_string(m_mux_index) + sep);
	printf("%s\r\n", out.c_str());
}


void Knob::init(uint8_t led_index, uint8_t mux_index, Pwm &pwm, uint16_t &mux_data, uint16_t max_val, uint8_t leds_count) {

	knob_leds_count = leds_count;
	knob_max_val = max_val;

	leds = &pwm;
	m_mux_data = &mux_data;
	m_first_led = led_index;
	m_mux_index = mux_index;

	encoder.init(*m_mux_data, m_mux_index+1);

	encoder.update();

	encoder.set(0);
	divider = (float)(knob_leds_count-1) / 127.;
	val_divider = 127. / (float)knob_max_val ;

	std::string sep(" ");
	std::string out(
			std::to_string(knob_leds_count) + sep +
			std::to_string(knob_max_val) + sep +
			std::to_string((uint32_t)leds) + sep +
			std::to_string((uint32_t)m_mux_data) + sep +
			std::to_string(m_first_led) + sep +
			std::to_string(m_mux_index) + sep);
	printf("%s\r\n", out.c_str());
}


knob_msg Knob::update() {

	knob_msg ret;


	bool sw = CHECKBIT(*m_mux_data, m_mux_index);



	if (last_sw != sw) {
		ret.switch_changed = true;
		last_sw = sw;
	}

	bool change = encoder.update();

	if (change) {
		int16_t enc = encoder.get();

		if (enc_last > enc) {
			if (encoder_value != 0)
				encoder_value--;

		} else {
			if (encoder_value < knob_max_val)
				encoder_value++;
		}

		enc_last = enc;

		ret.value_changed=true;
	}
	return ret;
}

int16_t Knob::get_value(){
	return encoder_value;
}

int16_t Knob::get_value_scaled(){
	return encoder_value * val_divider;
}

void Knob::set_value(uint16_t val){
	encoder_value = val / val_divider;
}

bool Knob::get_sw_state(){
	return last_sw;
}

void Knob::led_on(size_t led_nr, int16_t bright) {
	led_nr = (knob_leds_count-1) - led_nr;
	led_nr += m_first_led;
	leds->set(led_last, 0);
	leds->set(led_nr, bright);
	led_last = led_nr;
}
void Knob::set_leds(uint16_t value) {
	int led_bright = 256;
	int led_nr = value * divider;
	led_on(led_nr, led_bright);
}

