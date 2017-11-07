/*
 * Pwm.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Pwm.h"

static uint16_t pwm_array[PWM_DRIVERS_COUNT * PWM_COUNT] = { };

Pwm::Pwm() {
}

Pwm::~Pwm() {
}

int dupa[100]={};
int count_=0;

void Pwm::init() {

	pwm[0].i2c_probe();

	for(int i=0; i < PWM_DRIVERS_COUNT; i++)
	{
		pwm[i].set_address(dupa[i]);
		pwm[i].begin();
	}


	clear();
	update_all();

}

void Pwm::set(size_t led, uint16_t val) {
	pwm_array[led] = val;

	size_t dev_nr = led / PWM_COUNT;
	size_t dev_led = led % PWM_COUNT;

	pwm[dev_nr].setPWM(dev_led, 0, val);
}

void Pwm::update_all() {
	for (uint8_t j = 0; j < PWM_DRIVERS_COUNT; j++) {
		for (uint8_t i = 0; i < PWM_COUNT; i++) {
			pwm[j].setPWM(i, 0, pwm_array[(j * PWM_COUNT) + i]);
		}
	}
}

void Pwm::clear() {
	memset(pwm_array, 0, sizeof(pwm_array));
}

