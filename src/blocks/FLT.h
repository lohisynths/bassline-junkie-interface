/*
 * FLT.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_FLT_H_
#define SRC_FLT_H_

#include "UI_BLOCK.h"

#define FLT_KNOB_COUNT				(2)
#define FLT_BUTTON_COUNT			(3)
#define FLT_COUNT					(1)

#define FLT_FIRST_ENC_LED			(9*16)

#define FLT_FIRST_BUTTON_LED		(FLT_FIRST_ENC_LED+28)

#define FLT_MIDI_OFFSET				(32)

enum FLT_PARAMS {
	FLT_FREQ,
	FLT_RES,
	FLT_PARAM_COUNT
};

class FLT : public UI_BLOCK<FLT_KNOB_COUNT, FLT_BUTTON_COUNT, FLT_PARAM_COUNT, FLT_COUNT> {
public:
	~FLT(){};

	virtual const char* get_name()
	{
	    return "FLT";
	}

	typedef std::array<std::array<int, FLT_COUNT>, FLT_PARAM_COUNT> flt_preset;

	flt_preset &get_preset() {
		return knob_values;
	}

	void select_instance(uint8_t index) {
		auto &sw = get_sw();
		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current OSC and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < FLT_KNOB_COUNT; i++) {
			uint8_t val = knob_values[i][current_instance];
			knob_val_changed(i, val, true);
		}
		DEBUG_LOG("%s %d SELECTED\r\n", get_name(), index);
	};

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t big_knob_led_count = knob_led_count + 2;
		uint8_t knob_max_val = COMMON_KNOB_LED_COUNT;
		uint8_t big_knob_max_val = 127;
		uint16_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t button_max_led_val = BUTTON_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(3), 4, big_knob_max_val, leds, knob_led_max_val, (FLT_FIRST_ENC_LED + 0), big_knob_led_count},
			Knob::knob_init_map{mux, mux->get(3), 7, knob_max_val , leds, knob_led_max_val, (FLT_FIRST_ENC_LED + 16), knob_led_count}
		};

		button_config button_ctrl={
			Button::button_init_map{mux, mux->get(3), 15, leds, button_max_led_val, (FLT_FIRST_BUTTON_LED+2)},
			Button::button_init_map{mux, mux->get(3), 14, leds, button_max_led_val, (FLT_FIRST_BUTTON_LED+1)},
			Button::button_init_map{mux, mux->get(3), 13, leds, button_max_led_val, (FLT_FIRST_BUTTON_LED+0)}
		};
		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	virtual void button_changed(uint8_t index, bool state) {
		if (state) {
			if (index != current_instance) {
				select_instance(index);
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

	uint8_t get_midi_nr(uint8_t index) {
		return FLT_MIDI_OFFSET+index;
	}

private:

};


#endif /* SRC_FLT_H_ */
