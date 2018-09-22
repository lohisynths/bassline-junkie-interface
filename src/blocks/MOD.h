/*
 * MOD.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_MOD_H_
#define SRC_MOD_H_

#include "UI_BLOCK.h"

#define MOD_KNOB_COUNT				(1)
#define MOD_BUTTON_COUNT			(6)
#define MOD_COUNT					(6)

#define MOD_FIRST_ENC_LED			(7*16)

#define MOD_FIRST_BUTTON_LED		(MOD_FIRST_ENC_LED+10)

#define MOD_MIDI_OFFSET 			(96)

#define MOD_PARAM_COUNT 			(MOD_KNOB_COUNT)

class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_PARAM_COUNT, MOD_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MOD_LOG_LEVEL> LOG;

	~MOD(){};

	virtual const char* get_name() {
	    return "MOD";
	}

	void select_mode(uint8_t index) {
	    LOG::LOG0("MOD SE:ECT MODE %d\r\n", index);
	}

	void force_mode(uint8_t index) {
	    LOG::LOG0("MOD FORCE MODE %d\r\n", index);
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_max_val = KNOB_MAX_VAL;
		uint16_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t button_max_led_val = BUTTON_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(2), 0, knob_max_val, leds, knob_led_max_val, (MOD_FIRST_ENC_LED +  0), knob_led_count},
		};

		button_config button_ctrl={
			Button::button_init_map{mux, mux->get(2), 8, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 5)},
			Button::button_init_map{mux, mux->get(2), 7, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 4)},
			Button::button_init_map{mux, mux->get(2), 6, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 3)},
			Button::button_init_map{mux, mux->get(2), 5, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 2)},
			Button::button_init_map{mux, mux->get(2), 4, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 1)},
			Button::button_init_map{mux, mux->get(2), 3, leds, button_max_led_val, (MOD_FIRST_BUTTON_LED + 0)}

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
		return current_instance + (mod_dest*MOD_COUNT);
	}

	void select_MOD_dest(int index) {
		if(index > -1) {
		    LOG::LOG0("MOD dst %d\r\n", index);
			mod_dest = index;
		}
	}

private:
	uint8_t mod_dest = 0;
};

#endif /* SRC_MOD_H_ */
