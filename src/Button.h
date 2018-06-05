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

	void init(uint8_t led_index, uint8_t mux_index, Pwm &pwm, uint16_t &mux_data);

	bool update();

	bool get();

	void set_led_val(uint16_t val);

private:
	Pwm *m_leds=nullptr;
	uint16_t *m_mux_data;
	uint8_t led=0;
	uint8_t sw_bit=0;

	uint8_t last_sw=1;

	int bright = 2048;

};

#endif /* SRC_BUTTON_H_ */
