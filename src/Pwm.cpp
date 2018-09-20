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

void Pwm::init() {
	auto *addr = PCA9685::i2c_probe();

	for(int i=0; i < PWM_DRIVERS_COUNT; i++)
	{
		pwm[i].set_address(addr[i]);
		pwm[i].begin();
		pwm[i].setOpenDrain(false);
	}

	pwm[11].setOpenDrain(true);
	pwm[12].setOpenDrain(true);

	clear();
	update_all();

}

void Pwm::set(uint8_t led, uint16_t val) {
	pwm_array[led] = val;

	uint8_t dev_nr = led / PWM_COUNT;
	uint8_t dev_led = led % PWM_COUNT;

	pwm[dev_nr].setPWM(dev_led, 0, val);
}

void Pwm::update_all() {
	for (uint8_t j = 0; j < PWM_DRIVERS_COUNT; j++) {
		for (uint8_t idx = 0; idx < PWM_COUNT; idx++) {
			uint16_t val = pwm_array[(j * PWM_COUNT) + idx];
			//if(val > 0) {
				pwm[j].setPWM(idx, 0, val);
			//}
		}
	}
}

void Pwm::clear() {
	memset(pwm_array, 0, sizeof(pwm_array));
}

void Pwm::set(const uint16_t input[PWM_DRIVERS_COUNT * PWM_COUNT]) {
	memcpy(pwm_array, input, sizeof(pwm_array));
	update_all();
}

const uint16_t *Pwm::get() {
	return pwm_array;
}
