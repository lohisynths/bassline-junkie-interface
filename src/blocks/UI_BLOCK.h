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
#include <array>

template<uint8_t KNOB_COUNT, uint8_t BUTTON_COUNT, uint8_t PARAM_COUNT, uint8_t COUNT>
class UI_BLOCK {
public:
	typedef std::array<std::array<int, COUNT>, PARAM_COUNT> preset;
	typedef std::array<Knob::knob_init_map, KNOB_COUNT> knob_config;
	typedef std::array<Button::button_init_map, BUTTON_COUNT> button_config;

	UI_BLOCK() {};
	virtual ~UI_BLOCK(){};

	void init_internal(knob_config &knob_settings, button_config &button_config) {
		for (int i = 0; i < KNOB_COUNT; i++) {
			knob[i].init(knob_settings[i]);
		}
		for (int i = 0; i < BUTTON_COUNT; i++) {
			sw[i].init(button_config[i]);
		}
	}

	virtual void button_changed(uint8_t index, bool state) {
		if (state) {
			// if COUNT = 1
			// only one knob used, so jump straight to special_function
			if (index < COUNT && COUNT != 1) {
				select_instance(index);
			} else {
				special_function(index, state);
			}
		}
	};

	void update_buttons() {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			bool ret = sw[i].update();
			if (ret) {
				bool pushed = !sw[i].get_state();
				DEBUG_LOG("%s %d button switch %d", get_name(), current_instance, i);
				DEBUG_LOG( (pushed) ? " pushed\r\n" : " released\r\n" );
				button_changed(i, pushed);
			}
		}
	};

	int update_knobs() {
		int ret_val = -1;
		for (uint8_t i = 0; i < KNOB_COUNT; i++) {
			Knob::knob_msg ret = knob[i].update();
			if (ret.switch_changed) {
				bool state = !knob[i].get_switch_state();
				DEBUG_LOG("%s %d encoder switch %d ", get_name(), current_instance, i);
				DEBUG_LOG( (state) ? " pushed\r\n" : " released\r\n" );
				knob_sw_changed(i, state);
				if (state) {
					ret_val = i;
				}
			}
			if (ret.value_changed) {
				DEBUG_LOG("%s %d value %d changed %d\r\n", get_name(), current_instance, i, knob[i].get_knob_value());
				knob_val_changed(i, knob[i].get_knob_value());
			}
		}
		return ret_val;
	}

	void select_instance(uint8_t index) {
		auto &sw = get_sw();
		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current OSC and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < KNOB_COUNT; i++) {
			uint8_t val = preset_values[i][current_instance];
			knob_val_changed(i, val, true);
		}
		DEBUG_LOG("%s %d SELECTED\r\n", get_name(), index);
	};


	int update() {
		update_buttons();
		return update_knobs();
	}

	void set_preset(preset input) {
		memcpy(&preset_values, &input, sizeof(input));
		select_instance(current_instance);
	};

	void knob_val_changed(uint8_t index, uint16_t value_scaled, bool force_led_update = false) {
		knob[index].led_indicator_set_value(value_scaled, force_led_update);
		preset_values[index][current_instance] = value_scaled;
		midi->send_cc(get_midi_nr(index), value_scaled, 1);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) = 0;
	virtual void special_function(uint8_t index, uint8_t value) = 0;

	std::array<Button, BUTTON_COUNT> &get_sw(){return sw;} ;
	std::array<Knob, KNOB_COUNT> &get_knobs(){return knob;};
	uint8_t current_instance = 0;

	virtual const char* get_name() = 0;
	virtual uint8_t get_midi_nr(uint8_t index) = 0;

	MIDI *midi;
	int sw_bright = 1024;
	std::array<Button, BUTTON_COUNT> sw;
	preset preset_values = {};

private:
	std::array<Knob, KNOB_COUNT> knob;
	int led_bright = 256;

};

#endif /* SRC_ADSRCTL_H_ */
