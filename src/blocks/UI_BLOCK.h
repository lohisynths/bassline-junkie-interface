/*
 * ADSRCTL.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ADSRCTL_H_
#define SRC_ADSRCTL_H_

#include "utils.h"

#include "Knob.h"
#include "Button.h"
#include "Button.h"
#include <array>

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

template<uint8_t KNOB_COUNT, uint8_t BUTTON_COUNT, uint8_t PARAM_COUNT, uint8_t COUNT>
class UI_BLOCK {
public:
	typedef std::array<std::array<int, COUNT>, PARAM_COUNT> preset;

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
				DEBUG_LOG("%s %d button switch %d", get_name(), current_instance, i);
				DEBUG_LOG( (pushed) ? " pushed\r\n" : " released\r\n" );
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
				DEBUG_LOG("%s %d encoder switch %d ", get_name(), current_instance, i);
				DEBUG_LOG( (state) ? " pushed\r\n" : " released\r\n" );
				knob_sw_changed(i, state);
				if (state) {
					ret_val = i;
				}
			}
			if (ret.value_changed) {
				DEBUG_LOG("%s %d value %d changed %d\r\n", get_name(), current_instance, i, knob[i].get_value_scaled());

				knob_val_changed(i, knob[i].get_value_scaled());
			}
		}
		return ret_val;
	}

	int update() {
		update_buttons();
		return update_knobs();
	}

	void set_preset(preset input) {
		memcpy(&knob_values, &input, sizeof(input));
		select_instance(current_instance);
	};

	void knob_val_changed(uint8_t index, uint16_t value_scaled) {
		knob[index].set_leds(value_scaled);
		knob_values[index][0] = value_scaled;
		midi->send_cc(get_midi_nr(index), value_scaled, 1);
	}

	virtual void button_changed(uint8_t index, bool state) = 0;
	virtual void knob_sw_changed(uint8_t index, bool state) = 0;
	virtual void select_instance(uint8_t index) = 0;

	std::array<Button, BUTTON_COUNT> &get_sw(){return sw;} ;
	std::array<Knob, KNOB_COUNT> &get_knobs(){return knob;};
	std::array<std::array<int, COUNT>, PARAM_COUNT> knob_values = {};
	uint8_t current_instance = 0;

	virtual const char* get_name() = 0;
	virtual uint8_t get_midi_nr(uint8_t index) = 0;

	std::array<Knob, KNOB_COUNT> knob;


	int led_bright = 256;
	int sw_bright = 1024;
	MIDI *midi;

private:
	std::array<Button, BUTTON_COUNT> sw;

};

#endif /* SRC_ADSRCTL_H_ */
