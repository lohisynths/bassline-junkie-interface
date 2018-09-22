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

#define OSC_FIRST_BUTTON_LED		(6*16+(12))

#define OSC_MIDI_OFFSET 			(1)


enum OSC_PARAMS {
	OSC_PITCH,
	OSC_SIN,
	OSC_SAW,
	OSC_SQR,
	OSC_RND,
	OSC_PARAM_COUNT
};

class OSC : public UI_BLOCK<OSC_KNOB_COUNT, OSC_BUTTON_COUNT, OSC_PARAM_COUNT, OSC_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<set_level(LOG_LEVELS::DISABLED)> LOG;

	OSC(){};
	~OSC(){};

	virtual const char* get_name() {
	    return "OSC";
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;

		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_val_max_val = KNOB_MAX_LED_VAL;
		uint16_t knob_max_val = 64;
		uint16_t button_val_max_val = KNOB_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(1), 12,knob_max_val, leds,knob_val_max_val, (OSC_FIRST_ENC_LED + 48),knob_led_count},
			Knob::knob_init_map{mux, mux->get(1), 9, knob_max_val, leds,knob_val_max_val, (OSC_FIRST_ENC_LED + 30),knob_led_count},
			Knob::knob_init_map{mux, mux->get(1), 6, knob_max_val, leds,knob_val_max_val, (OSC_FIRST_ENC_LED + 20),knob_led_count},
			Knob::knob_init_map{mux, mux->get(1), 3, knob_max_val, leds,knob_val_max_val, (OSC_FIRST_ENC_LED + 10),knob_led_count},
			Knob::knob_init_map{mux, mux->get(1), 0, knob_max_val, leds,knob_val_max_val, (OSC_FIRST_ENC_LED + 0) ,knob_led_count}
		};

		button_config button_ctrl={
			Button::button_init_map{mux, mux->get(3),3, leds, button_val_max_val, (OSC_FIRST_BUTTON_LED+2)},
			Button::button_init_map{mux, mux->get(3),2, leds, button_val_max_val, (OSC_FIRST_BUTTON_LED+1)},
			Button::button_init_map{mux, mux->get(1),15, leds,button_val_max_val, (OSC_FIRST_BUTTON_LED)}};

		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void select_mode(uint8_t index) {
	    LOG::LOG0("%s %d special_function %d\r\n", get_name(), current_instance, index);
	}

	void force_mode(uint8_t index) {
	    LOG::LOG0("%s %d forced %d\r\n", get_name(), current_instance, index);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

	uint8_t get_midi_nr(uint8_t index) {
		return OSC_MIDI_OFFSET+index+(current_instance * (OSC_KNOB_COUNT+1));
	}

	uint8_t get_current_osc() { return current_instance; };

	void set_preset(preset input) {
		memcpy(&preset_values, &input, sizeof(input));
		select_instance(current_instance);
	};

private:

};


#endif /* SRC_OSC_H_ */
