/*
 * Knob.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Knob.h"
#include "utils.h"

struct knob_map {
	uint8_t enc_bit;
	uint8_t first_led;
	uint8_t sw_bit;
};

static const struct knob_map knob_mapping[KNOB_COUNT] = {
	{ 6, 30, 3 },
	{ 4, 20, 2 },
	{ 2, 10, 1 },
	{ 0, 0, 0 },

	{ 14, 78, 8 },
	{ 12, 68, 9 },
	{ 10, 58, 10 },
	{ 8, 48, 11 },

	{ 16, 96, 16 }
};

Knob::Knob() {
}

Knob::~Knob() {
}

void Knob::init(size_t index, Pwm *pwm, int32_t mux_data) {
	leds = pwm;

	first_led = knob_mapping[index].first_led;
	enc_a_bit = knob_mapping[index].enc_bit + 1;
	enc_b_bit = knob_mapping[index].enc_bit;

	sw_bit = knob_mapping[index].sw_bit;

	encoder.update(
			CHECKBIT(mux_data, enc_a_bit),
			CHECKBIT(mux_data, enc_b_bit));

	encoder.set(0);
	led_on(0, 512);

}


knob_msg Knob::update(int32_t mux_data, int32_t sw_data) {
	knob_msg ret;

	bool sw = CHECKBIT(sw_data, sw_bit);

	if (last_sw != sw) {
		ret.switch_changed = true;
		last_sw = sw;
	}

	bool change = encoder.update(
		CHECKBIT(mux_data, enc_a_bit),
		CHECKBIT(mux_data, enc_b_bit));

	if (change) {
		int16_t enc = encoder.get();

		if (enc_last > enc) {
			if (encoder_value != 0)
				encoder_value--;

		} else {
			if (encoder_value < 63)
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

void Knob::set_value(int16_t val){
	encoder_value = val;
}

bool Knob::get_sw_state(){
	return last_sw;
}

void Knob::led_on(size_t led_nr, int16_t bright) {
	led_nr = 9 - led_nr;
	led_nr += first_led;
	leds->set(led_last, 0);
	leds->set(led_nr, bright);
	led_last = led_nr;
}

