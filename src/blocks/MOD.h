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

// MOD_COUNT = 6
// ADSR0, ADSR1, ADSR2, LFO0, LFO1, LFO2

// OSC0 pitch, OSC0 sin, OSC0 saw, OSC0 sqr, OSC0 rnd          = 5
// OSC1 pitch, OSC1 sin, OSC1 saw, OSC1 sqr, OSC1 rnd          = 5
// OSC2 pitch, OSC2 sin, OSC2 saw, OSC2 sqr, OSC2 rnd          = 5
// FLT freq, FLT reso                                          = 2
//                                                               17
#define  MOD_PARAM_COUNT 17


class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_PARAM_COUNT, MOD_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MOD_LOG_LEVEL> LOG;

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_max_val = KNOB_MAX_VAL;
		uint16_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t button_max_led_val = BUTTON_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(2), 0, knob_max_val,
			                    leds, knob_led_max_val, (MOD_FIRST_ENC_LED +  0), knob_led_count},
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

    virtual ~MOD(){};

    virtual const char* get_name() {
        return "MOD";
    }

    virtual int16_t get_current_preset_value(uint8_t index) {
        return preset_values[current_instance][actual_mod_dest];
    }

    virtual void set_current_preset_value(uint8_t index, uint16_t value) {
        preset_values[current_instance][actual_mod_dest] = value;
    }

    void select_MOD_dest(int index) {
        LOG::LOG0("%s dst %d selected\r\n", get_name(), index);
        actual_mod_dest = index;
    }

    virtual void select_mode(uint8_t index) { }
    virtual void force_mode(uint8_t index) { }
    virtual void knob_sw_changed(uint8_t index, bool state) { }

    virtual uint8_t get_midi_ch(){
        return 2;
    }

    virtual uint8_t get_midi_nr(uint8_t instance, uint8_t index) {
        return instance + (index*MOD_COUNT);
    }

    virtual uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(current_instance, index);
    }

private:
	uint8_t actual_mod_dest = 0;
};

#endif /* SRC_MOD_H_ */
