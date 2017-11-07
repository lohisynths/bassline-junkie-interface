/*
 * Button.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#include "mbed.h"
#include "Pwm.h"
#include "utils.h"

class Button {
public:
	Button();
	virtual ~Button();

	void init(size_t index, Pwm *pwm, int16_t mux_data);

	bool update(int16_t sw_data);

	bool get();

	void led_on(uint16_t val);

private:
	Pwm *leds=nullptr;

	uint8_t led=0;
	uint8_t sw_bit=0;

	uint8_t last_sw=1;

	int bright = 2048;

};

#endif /* SRC_BUTTON_H_ */
