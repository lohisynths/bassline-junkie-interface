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

#define FLT_MIDI_OFFSET				(33)

enum FLT_PARAMS {
	FLT_FREQ,
	FLT_RES,
	FLT_SHAPE,
	FLT_PARAM_COUNT
};

class FLT : public UI_BLOCK<FLT_KNOB_COUNT, FLT_BUTTON_COUNT, FLT_PARAM_COUNT, FLT_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<FLT_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

	virtual const char* get_name() {
	    return "FLT";
	}

    uint8_t get_midi_ch(){
        return 1;
    }

	void init() {
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t big_knob_led_count = knob_led_count + 2;
		uint8_t knob_max_val = KNOB_MAX_VAL;
		uint8_t big_knob_max_val = 127;
		uint16_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t button_max_led_val = BUTTON_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{3, 4, big_knob_max_val, knob_led_max_val, (FLT_FIRST_ENC_LED + 0), big_knob_led_count},
			Knob::knob_init_map{3, 7, knob_max_val , knob_led_max_val, (FLT_FIRST_ENC_LED + 16), knob_led_count}
		};

		button_config button_ctrl={
			Button::button_init_map{3, 15, button_max_led_val, (FLT_FIRST_BUTTON_LED+2)},
			Button::button_init_map{3, 14, button_max_led_val, (FLT_FIRST_BUTTON_LED+1)},
			Button::button_init_map{3, 13, button_max_led_val, (FLT_FIRST_BUTTON_LED+0)}
		};
		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void select_mode(uint8_t index) {
			select_filter_type(index);
	}

	void force_mode(uint8_t index) {
			select_filter_type(index, true);
	}

	void select_filter_type(uint8_t index, bool force = 0) {
		if(last_filter_type != index || force) {
		    LOG::LOG0("%s type %d selected\r\n", get_name(), index);
			turn_off_sw(last_filter_type);
			turn_on_sw(index);
			set_current_preset_value(FLT_SHAPE, index);
			last_filter_type = index;
		}
	}

	void knob_sw_changed(uint8_t index, bool state) {

	}

    uint8_t get_midi_nr(uint8_t instance, uint8_t index) {
        return FLT_MIDI_OFFSET+index;
    }

    uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(current_instance, index);
    }

private:
	uint8_t last_filter_type = 0;
};


#endif /* SRC_FLT_H_ */
