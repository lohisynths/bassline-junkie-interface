/*
 * Display.h
 *
 *  Created on: Jun 10, 2018
 *      Author: alax
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#define DISP_FIRST_LED			(3*16)
#include "Pwm.h"


class Display {
public:
	Display() :index(0), m_leds(NULL){};
	virtual ~Display() {};

	void init(uint8_t led_index, Pwm &pwm) {
		index = led_index;
		m_leds = &pwm;
	}

	void set(uint8_t input) {
		uint8_t first_number = input%10;
		uint8_t second_number = input / 10;
		uint8_t third_number = input / 100;
	}


	uint8_t index;
	Pwm *m_leds;
};

#endif /* SRC_DISPLAY_H_ */
