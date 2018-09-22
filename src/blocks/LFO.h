/*
 * LFO.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_LFO_H_
#define SRC_LFO_H_

#include "UI_BLOCK.h"

#define LFO_KNOB_COUNT				(1)
#define LFO_BUTTON_COUNT			(8)
#define LFO_COUNT					(3)

#define LFO_FIRST_ENC_LED			(8*16)

#define LFO_FIRST_BUTTON_LED		(LFO_FIRST_ENC_LED+10)

#define LFO_MIDI_OFFSET				(36)

enum LFO_PARAMS {
	LFO_FREQ,
	LFO_SHAPE,
	LFO_PARAM_COUNT
};
/*
LFO_OFFSET 36	0 + LFO_PARAMS*0	36	LFO 0 SHAPE
	1 + LFO_PARAMS*0	37	LFO 0 FREQ
	0 + LFO_PARAMS*1	38	LFO 1 SHAPE
	1 + LFO_PARAMS*1	39	LFO 1 FREQ
	0 + LFO_PARAMS*2	40	LFO 2 SHAPE
	1 + LFO_PARAMS*2	41	LFO 2 FREQ
*/
/*
LFO_OFFSET 36
	0 + LFO_PARAMS*0	36	LFO 0 SHAPE
	1 + LFO_PARAMS*0	37	LFO 0 FREQ
	0 + LFO_PARAMS*1	38	LFO 1 SHAPE
	1 + LFO_PARAMS*1	39	LFO 1 FREQ
	0 + LFO_PARAMS*2	40	LFO 2 SHAPE
	1 + LFO_PARAMS*2	41	LFO 2 FREQ
*/

class LFO : public UI_BLOCK<LFO_KNOB_COUNT, LFO_BUTTON_COUNT, LFO_PARAM_COUNT, LFO_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<set_level(LOG_LEVELS::DISABLED)> LOG;

	~LFO(){};

	virtual const char* get_name() {
	    return "LFO";
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_max_val = COMMON_KNOB_LED_COUNT;
		uint16_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t button_max_led_val = BUTTON_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(4), 0, knob_max_val, leds, knob_led_max_val, (LFO_FIRST_ENC_LED +  0), knob_led_count},
		};

		button_config button_ctrl={
			Button::button_init_map{mux, mux->get(3), 0,  leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 20)},
			Button::button_init_map{mux, mux->get(2), 15, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 19)},
			Button::button_init_map{mux, mux->get(2), 14, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 18)},

			Button::button_init_map{mux, mux->get(2), 13, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 4)},
			Button::button_init_map{mux, mux->get(2), 12, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 3)},
			Button::button_init_map{mux, mux->get(2), 11, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 2)},
			Button::button_init_map{mux, mux->get(2), 10, leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 1)},
			Button::button_init_map{mux, mux->get(2), 9,  leds, button_max_led_val, (LFO_FIRST_BUTTON_LED + 0)}
		};
		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void select_lfo_shape(uint8_t index, bool force = 0) {
		if(last_lfo_shape != index || force) {
		    LOG::LOG0("%s SELECT MOD %d\r\n", get_name(), index);

			uint8_t shape_button_adr = index + LFO_COUNT;
			uint8_t last_shape_button_adr = last_lfo_shape + LFO_COUNT;

			turn_off_sw(last_shape_button_adr);
			turn_on_sw(shape_button_adr);

			set_current_preset_value(LFO_SHAPE, index);
			last_lfo_shape = index;
		}
	}

	void select_mode(uint8_t index) {
		select_lfo_shape(index);
	}

	void force_mode(uint8_t index) {
	    LOG::LOG0("%s FORCE MODE %d\r\n", get_name(), index);
		select_lfo_shape(index, true);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {}

	uint8_t get_midi_nr(uint8_t index) {
		return LFO_MIDI_OFFSET+index+ 1 + (current_instance*2);
	}

private:
	uint8_t last_lfo_shape = 0;
};


#endif /* SRC_LFO_H_ */
