/*
 * PRESET.h
 *
 *  Created on: Jun 9, 2018
 *      Author: alax
 */

#ifndef SRC_BLOCKS_LED_DISP_H_
#define SRC_BLOCKS_LED_DISP_H_

#include "utils.h"

#include "Knob.h"
#include "Button.h"

#define PRESET_KNOB_COUNT				(1)
#define PRESET_BUTTON_COUNT				(0)
#define PRESET_COUNT					(1)

#define PRESET_FIRST_ENC_LED			(11*16)
#define SEGMENTS						(8)

#define PRESET_PARAM_COUNT				(PRESET_KNOB_COUNT)

class LED_DISP : public UI_BLOCK<PRESET_KNOB_COUNT, PRESET_BUTTON_COUNT, PRESET_PARAM_COUNT, PRESET_COUNT> {
public:

    using UI_BLOCK::UI_BLOCK;

    bool if_knob_sw_pushed(){
	    auto &knobs = get_knobs();
	    return !knobs[0].get_switch_state();
	}
	virtual const char* get_name() {
	    return "Disp";
	}

    uint8_t get_midi_ch(){
        return 1;
    }


    uint8_t get_midi_nr(uint8_t instance, uint8_t index) {
        return get_midi_ch();
    }

	void button_changed(uint8_t index, bool state) {};

	void select_instance(uint8_t index) {};

    virtual void knob_val_changed(uint8_t index, uint16_t value_scaled) {
        LOG::LOG0("%s value changed %d\r\n", get_name(), value_scaled);
        actual_preset_value = value_scaled;

		uint8_t first_nr  = value_scaled % 10;
		uint8_t second_nr = (value_scaled / 10) % 10;
		uint8_t third_nr  = value_scaled / 100;

		set_digit(2, first_nr);
		set_digit(1, second_nr);
		set_digit(0, third_nr);
	}

	bool digits[10][SEGMENTS] {
		1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 0, 0, 0, 0, 0,
		1, 1, 0, 1, 1, 0, 1, 0,
		1, 1, 1, 1, 0, 0, 1, 0,
		0, 1, 1, 0, 0, 1, 1, 0,

		1, 0, 1, 1, 0, 1, 1, 0,
		1, 0, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 0, 1, 1, 0
	};

	void set_digit(uint8_t seg_nr, uint8_t digit) {
        LOG::LOG0("%s set digit %d %d\r\n", get_name(), seg_nr, digit);

		for(int i=0;i<7;i++) {
		    get_leds()->set(PRESET_FIRST_ENC_LED+i+(seg_nr*SEGMENTS), digits[digit][i]*DISPLAY_MAX_LED_VAL);
		}
	}

	void set_all(uint16_t val) {
        LOG::LOG0("%s set all %d\r\n", get_name(), val);
		for(int i=0;i<SEGMENTS*3;i++) {
		    get_leds()->set(PRESET_FIRST_ENC_LED+i, val);
		}
	}

	int last_led = PRESET_FIRST_ENC_LED;

    void select_function(uint8_t index) {
        LOG::LOG0("%s %d special_function %d\r\n", get_name(), get_current_instasnce(), index);
    }

    void force_function(uint8_t index) {
        LOG::LOG0("%s %d forced %d\r\n", get_name(), get_current_instasnce(), index);
    }

    uint8_t get_actual_preset_nr() {
        return actual_preset_value;
    }

    uint8_t actual_preset_value = 0;
};

#endif /* SRC_BLOCKS_LED_DISP_H_ */
