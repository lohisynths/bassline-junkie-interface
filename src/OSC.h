/*
 * OSC.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_OSC_H_
#define SRC_OSC_H_

#include "UI_BLOCK.h"

#define OSC_KNOB_COUNT				(5)
#define OSC_BUTTON_COUNT			(3)
#define OSC_COUNT					(3)

#define OSC_FIRST_ENC_LED			(3*16)
#define OSC_FIRST_ENC_MUX_ADR		(0)

#define OSC_FIRST_BUTTON_LED		(6*16+(12))


class OSC : public UI_BLOCK<OSC_KNOB_COUNT, OSC_BUTTON_COUNT, OSC_COUNT> {
public:
	~OSC(){};

	char const *NAME = "OSC";
	enum OSC_BUTTONS {
		OSC0, OSC1, OSC2, LOOP
	};

	void init(Mux *mux, Pwm *leds) {

		knob_data OSC_ctl[OSC_KNOB_COUNT] = {
				OSC_FIRST_ENC_LED +  0, OSC_FIRST_ENC_MUX_ADR + 0,  mux->get(1),
				OSC_FIRST_ENC_LED + 10, OSC_FIRST_ENC_MUX_ADR + 3,  mux->get(1),
				OSC_FIRST_ENC_LED + 20, OSC_FIRST_ENC_MUX_ADR + 6,  mux->get(1),
				OSC_FIRST_ENC_LED + 30, OSC_FIRST_ENC_MUX_ADR + 9,  mux->get(1),
				OSC_FIRST_ENC_LED + 48, OSC_FIRST_ENC_MUX_ADR + 12, mux->get(1)
		};

		sw_data OSC_ctl_sw[OSC_BUTTON_COUNT] = {
				OSC_FIRST_BUTTON_LED,   15,    mux->get(1),
				OSC_FIRST_BUTTON_LED+1, 0 + 1, mux->get(0),
				OSC_FIRST_BUTTON_LED+2, 0 + 2, mux->get(0),
		};
		init_internal(*leds, OSC_ctl, OSC_ctl_sw);
		Button *sw = get_sw();
		sw[current_instance].set_led_val(sw_bright);
	}

	virtual void button_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d button switch %d", NAME, current_instance, index);
		if (state)
			DEBUG_LOG(" pushed\r\n");
		else
			DEBUG_LOG(" released\r\n");

		if (state) {
			if (index == LOOP) {
				select_loop(index, state);
			} else {
				if (index != current_instance) {
					select_OSC(index);
				}
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d encoder switch %d ", NAME, current_instance, index);
		if (state)
			DEBUG_LOG("pushed\r\n");
		else
			DEBUG_LOG("released\r\n");
	}

	virtual void knob_val_changed(uint8_t index, int value) {
		DEBUG_LOG("%s %d value %d changed %d\r\n", NAME, current_instance, index, value);

		Knob *knob = get_knobs();
		int16_t actual_value = knob[index].get_value();
		OSC_val[current_instance][index] = actual_value;

		int led_nr = actual_value / 7;
		knob[index].led_on(led_nr, led_bright);
	}

	void select_OSC(uint8_t index) {
		Button *sw = get_sw();

		sw[index].set_led_val(sw_bright);

		// get button number of button from current OSC and turn led off
		sw[current_instance].set_led_val(0);

		current_instance = index;

		for (int i = 0; i < OSC_KNOB_COUNT; i++) {
			Knob *knob = get_knobs();
			knob[i].set_value(OSC_val[current_instance][i]);

			int led_nr = knob[i].get_value() / 7;
			knob[i].led_on(led_nr, led_bright);
		}

		sw[LOOP].set_led_val(OSC_loop[current_instance] * sw_bright);

		DEBUG_LOG("%s %d SELECTED\r\n", NAME, index);
	};

	void select_loop(uint8_t index, bool loop) {
		OSC_loop[current_instance] ^= 1;
		Button *sw = get_sw();
		sw[index].set_led_val(OSC_loop[current_instance] * sw_bright);

		DEBUG_LOG("%s %d ", NAME, current_instance);
		if (OSC_loop[current_instance]) {
			DEBUG_LOG("LOOP ON\r\n");
		} else {
			DEBUG_LOG("LOOP OFF\r\n");
		}
	}


private:
	bool OSC_loop[OSC_COUNT]={};
	int16_t OSC_val[OSC_COUNT][OSC_KNOB_COUNT]={};

	int led_bright = 256;
	int sw_bright = 1024;
	uint8_t current_instance = 0;


};


#endif /* SRC_OSC_H_ */
