/*
 * Button.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Button.h"

struct sw_map {
	uint8_t led;
	uint8_t sw_bit;
};

static const struct sw_map sw_mapping[BUTTON_COUNT] = {
	{43, 7},
	{42, 6},
	{41, 5},
	{40, 4},

	{90, 13},
	{89, 14},
	{88, 15}
};

Button::Button() {

}

Button::~Button() {

}

void Button::init(size_t index, Pwm *pwm, int16_t mux_data) {
	leds = pwm;

	led = sw_mapping[index].led;
	sw_bit = sw_mapping[index].sw_bit;

}

bool Button::update(int16_t sw_data) {
	bool ret = false;

	bool sw = CHECKBIT(sw_data, sw_bit);

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

void Button::led_on(uint16_t val)
{
	leds->set(led, val);
}


