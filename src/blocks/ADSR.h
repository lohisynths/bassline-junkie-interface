/*
 * ADSR.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_ADSR_H_
#define SRC_ADSR_H_

#include "UI_BLOCK.h"

#define ADSR_KNOB_COUNT				(4)
#define ADSR_BUTTON_COUNT			(4)
#define ADSR_COUNT					(3)

#define ADSR_FIRST_ENC_LED			(0)

#define ADSR_FIRST_BUTTON_LED		(ADSR_KNOB_COUNT*10)

#define ADSR_MIDI_OFFSET			(18)

enum ADSR_PARAMS {
	ADSR_ATTACK,
	ADSR_DECAY,
	ADSR_SUSTAIN,
	ADSR_RELEASE,
	ADSR_LOOP,
	ADSR_PARAM_NR
};

class ADSR : public UI_BLOCK<ADSR_KNOB_COUNT, ADSR_BUTTON_COUNT, ADSR_PARAM_NR, ADSR_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<ADSR_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

	virtual const char* get_name() {
	    return "ADSR";
	}

	uint8_t get_midi_ch(){
	    return 1;
	}

	enum ADSR_BUTTONS {
		ADSR0, ADSR1, ADSR2, LOOP
	};

	void init() {
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_led_max_val = KNOB_MAX_LED_VAL;
		uint16_t knob_max_val = KNOB_MAX_VAL;
		uint16_t button_val_max_val = KNOB_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{0, 9, knob_max_val, knob_led_max_val, (ADSR_FIRST_ENC_LED + 30), knob_led_count},
			Knob::knob_init_map{0, 6, knob_max_val, knob_led_max_val, (ADSR_FIRST_ENC_LED + 20), knob_led_count},
			Knob::knob_init_map{0, 3, knob_max_val, knob_led_max_val, (ADSR_FIRST_ENC_LED + 10), knob_led_count},
			Knob::knob_init_map{0, 0, knob_max_val,  knob_led_max_val, (ADSR_FIRST_ENC_LED + 0),  knob_led_count},
		};

		button_config button_ctrl={
			Button::button_init_map{0, 15, button_val_max_val, (ADSR_FIRST_BUTTON_LED+3)},
			Button::button_init_map{0, 14, button_val_max_val, (ADSR_FIRST_BUTTON_LED+2)},
			Button::button_init_map{0, 13, button_val_max_val, (ADSR_FIRST_BUTTON_LED+1)},
			Button::button_init_map{0, 12, button_val_max_val, (ADSR_FIRST_BUTTON_LED+0)}
		};

		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void force_mode(uint8_t input) {
		uint8_t button_adr = ADSR_COUNT;

		set_current_preset_value(LOOP, input);
		if(input) {
			turn_on_sw(button_adr);
		} else {
			turn_off_sw(button_adr);
		}
	}

	void select_mode(uint8_t index) {
		// index ignored as ADSR has only one extra function
		// with no parameters.
		// for now we care only about triggering loop state for
		// current instance
		triger_loop_for_current_preset();
	}

	void triger_loop_for_current_preset() {
		// in Pwm address space ADSR 0-2 selection buttons
		// are first and then LOOP switch
		// so button addr = ADSR_COUNT
		uint8_t button_adr = ADSR_COUNT;
		uint8_t preset_adr = ADSR_KNOB_COUNT;

		int16_t loop_state = get_current_preset_value(preset_adr);
		loop_state ^= 1;
		if(loop_state) {
			turn_on_sw(button_adr);
		} else {
			turn_off_sw(button_adr);
		}
		set_current_preset_value(preset_adr, loop_state);
        LOG::LOG0("ADSR %d LOOP %d\r\n", current_instance, loop_state);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

    uint8_t get_midi_nr(uint8_t instance, uint8_t index){
        return ADSR_MIDI_OFFSET+index+(instance * ADSR_PARAM_NR);
    }

    uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(current_instance, index);
    }

private:

};

#endif /* SRC_ADSR_H_ */
