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
	typedef std::array<std::array<int, PARAM_COUNT>, COUNT> preset;
	typedef std::array<Knob::knob_init_map, KNOB_COUNT> knob_config;
	typedef std::array<Button::button_init_map, BUTTON_COUNT> button_config;

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<UI_BLOCK_LOG_LEVEL> LOG;

	UI_BLOCK() : midi(nullptr) {};
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
			// if COUNT = 1 only one knob used,
			// so jump straight to special_function
			// if i < COUNT then select instance
			// common behavior for every class with buttons
			if(COUNT != 1) {
				if (index < COUNT) {
					select_instance(index);
				} else {
					select_mode(index-COUNT);
				}
			} else {
				select_mode(index);
			}
		}
	};

	void knob_val_changed(uint8_t index, uint16_t value_scaled, bool force_led_update = false) {
		knob[index].led_indicator_set_value(value_scaled, force_led_update);
		set_current_preset_value(index, value_scaled);
		midi->send_cc(get_midi_nr(index), value_scaled, 1);
	}

	void force_knob_update(uint8_t index, uint16_t value_scaled) {
		knob[index].set_value(value_scaled);
		knob_val_changed(index, value_scaled, true);
	}

	void update_buttons() {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			bool ret = sw[i].update();
			if (ret) {
				bool pushed = !sw[i].get_state();
				LOG::LOG0("%s %d button switch %d", get_name(), current_instance, i);
				LOG::LOG0((pushed) ? "pushed\r\n" : "released\r\n" );
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
				LOG::LOG0("%s %d encoder switch %d ", get_name(), current_instance, i);
				LOG::LOG0( (state) ? "pushed\r\n" : "released\r\n" );
				knob_sw_changed(i, state);
				if (state) {
					ret_val = i;
				}
			}
			if (ret.value_changed) {
			    LOG::LOG0("%s %d value %d changed %d\r\n", get_name(), current_instance, i, knob[i].get_knob_value());
				knob_val_changed(i, knob[i].get_knob_value());
			}
		}
		return ret_val;
	}

	int16_t get_current_preset_value(uint8_t index) {
		return preset_values[current_instance][index];
	}

	void set_current_preset_value(uint8_t index, uint16_t value) {
		preset_values[current_instance][index] = value;
	}

	void select_instance(uint8_t index) {

		// force update for select instance
		// if index == current instance
		// turn off index led and turn on
		if(index != current_instance) {
			// get button number of button from current OSC and turn led off
			turn_off_sw(current_instance);
			turn_on_sw(index);
			current_instance = index;
		}

		for (int i = 0; i < KNOB_COUNT; i++) {
			// get value of [i] knob from current preset
			//
			uint8_t val = get_current_preset_value(i);
			force_knob_update(i, val);
		}

		const int special_parameters_count = PARAM_COUNT - KNOB_COUNT;
		for (int i = 0; i < special_parameters_count; i++) {
			uint8_t val = get_current_preset_value(KNOB_COUNT+i);
			force_mode(val);
			LOG::LOG0("%s current_instance %d preset %d value  %d \r\n", get_name(), current_instance, KNOB_COUNT+i, val);
		}

		LOG::LOG0("%s %d selected\r\n", get_name(), index);
	};


	int update() {
		update_buttons();
		return update_knobs();
	}

	void set_preset(preset input) {
		memcpy(&preset_values, &input, sizeof(input));
		select_instance(current_instance);
	};

	preset &get_preset() {
		return preset_values;
	}

	void turn_off_sw(uint8_t index) {
		sw[index].set_led_val(0);
	}
	void turn_on_sw(uint8_t index) {
		sw[index].set_led_val(BUTTON_MAX_LED_VAL);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) = 0;
	virtual void select_mode(uint8_t index) = 0;
	virtual void force_mode(uint8_t index) = 0;

	std::array<Button, BUTTON_COUNT> &get_sw(){return sw;} ;
	std::array<Knob, KNOB_COUNT> &get_knobs(){return knob;};
	uint8_t current_instance = 0;

	virtual const char* get_name() = 0;
	virtual uint8_t get_midi_nr(uint8_t index) = 0;

	MIDI *midi;
	std::array<Button, BUTTON_COUNT> sw;
	preset preset_values = {};

private:
	std::array<Knob, KNOB_COUNT> knob;

};

#endif /* SRC_ADSRCTL_H_ */
