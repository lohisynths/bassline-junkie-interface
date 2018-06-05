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

void Button::init(uint8_t led_index, uint8_t mux_index, Pwm &pwm, uint16_t &mux_data) {
	m_leds = &pwm;
	m_mux_data = &mux_data;

	led = led_index;
	sw_bit = mux_index;

}

bool Button::update() {
	bool ret = false;

	bool sw = CHECKBIT(*m_mux_data, sw_bit);

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
	m_leds->set(led, val);
}


