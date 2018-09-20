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
	FLT_SHAPE,
	FLT_PARAM_COUNT
};




typedef std::tuple<int, const char *> parameter_;



class FLT : public UI_BLOCK<FLT_KNOB_COUNT, FLT_BUTTON_COUNT, FLT_PARAM_COUNT, FLT_COUNT> {
public:
	~FLT(){};

	virtual const char* get_name() {
	    return "FLT";
	}

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

	void special_function_button_pressed(uint8_t index) {
		select_filter_type(index);
	}

	void select_filter_type(uint8_t index) {
		DEBUG_LOG("FILTER TYPE %d SELECTED\r\n", index);

		turn_off_sw(last_filter_type);
		turn_on_sw(index);
		set_current_preset_value(FLT_SHAPE, index);
		last_filter_type = index;
	}

	void knob_sw_changed(uint8_t index, bool state) {

	}

	uint8_t get_midi_nr(uint8_t index) {
		return FLT_MIDI_OFFSET+index;
	}

	preset &get_preset() {
		return preset_values;
	}

private:
	uint8_t last_filter_type = 0;
};


#endif /* SRC_FLT_H_ */
