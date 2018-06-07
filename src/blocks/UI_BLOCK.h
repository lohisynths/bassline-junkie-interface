/*
 * ADSRCTL.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ADSRCTL_H_
#define SRC_ADSRCTL_H_

#include "../utils.h"

#include "../Knob.h"
#include "../Button.h"

struct knob_data {
	uint8_t knobs_first_led;
	uint8_t knobs_first_mux_adr;
	uint16_t *knobs_mux_data;
};

struct sw_data {
	uint8_t sw_first_led;
	uint8_t sw_first_mux_adr;
	uint16_t *sw_mux_data;
};

template<uint8_t KNOB_COUNT, uint8_t BUTTON_COUNT, uint8_t COUNT>
class UI_BLOCK {
public:
	UI_BLOCK() {};
	virtual ~UI_BLOCK(){};

	void init_internal(Pwm &leds, knob_data knobdata[KNOB_COUNT],
			sw_data swdata[BUTTON_COUNT]) {
		for (int i = 0; i < KNOB_COUNT; i++) {
			knob[i].init(knobdata[i].knobs_first_led,
					knobdata[i].knobs_first_mux_adr, leds,
					*knobdata[i].knobs_mux_data, 64, 10);
		}
		for (int i = 0; i < BUTTON_COUNT; i++) {
			sw[i].init(swdata[i].sw_first_led, swdata[i].sw_first_mux_adr, leds,
					*swdata[i].sw_mux_data);
		}
	};

	void update_buttons() {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			bool ret = sw[i].update();
			if (ret) {
				bool pushed = !sw[i].get();
				button_changed(i, pushed);
			}
		}
	};

	int update_knobs() {
		int ret_val = -1;
		for (uint8_t i = 0; i < KNOB_COUNT; i++) {
			knob_msg ret = knob[i].update();
			if (ret.switch_changed) {
				bool state = !knob[i].get_sw_state();
				knob_sw_changed(i, state);
				if (state) {
					ret_val = i;
				}
			}
			if (ret.value_changed) {
				knob_val_changed(i, knob[i].get_value());
			}
		}
		return ret_val;
	}

	int update() {
		update_buttons();
		return update_knobs();
	}

	virtual void button_changed(uint8_t index, bool state) = 0;
	virtual void knob_sw_changed(uint8_t index, bool state) = 0;
	virtual void knob_val_changed(uint8_t index, int value) = 0;

	Button *get_sw(){return sw;} ;
	Knob *get_knobs(){return knob;};

private:
	Knob knob[KNOB_COUNT];
	Button sw[BUTTON_COUNT];

};

#endif /* SRC_ADSRCTL_H_ */
