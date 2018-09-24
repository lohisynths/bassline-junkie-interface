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
	LED_DISP(){};
	virtual ~LED_DISP(){};

	virtual const char* get_name() {
	    return "Disp";
	}

    uint8_t get_midi_ch(){
        return 1;
    }

	void button_changed(uint8_t index, bool state) {};

	void select_instance(uint8_t index) {};

	uint8_t get_midi_nr(uint8_t index) {
		return 0;
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

    virtual void knob_val_changed(uint8_t index, uint16_t value_scaled, bool force_led_update = false) {
        LOG::LOG0("%s value changed %d\r\n", get_name(), value_scaled);

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
			m_leds->set(PRESET_FIRST_ENC_LED+i+(seg_nr*SEGMENTS), digits[digit][i]*DISPLAY_MAX_LED_VAL);
		}

//		m_leds->set(PRESET_FIRST_ENC_LED+digit, 1024);
//		m_leds->set(last_led, 0);
//
//		last_led = PRESET_FIRST_ENC_LED+digit;
	}

	void set_all(uint16_t val) {
        LOG::LOG0("%s set all %d\r\n", get_name(), val);
		for(int i=0;i<SEGMENTS*3;i++) {
			m_leds->set(PRESET_FIRST_ENC_LED+i, val);
		}
	}

    void init(Mux *mux, Pwm *leds, MIDI *midi_) {
        LOG::LOG0("%s init\r\n", get_name());
        uint8_t knob_led_max_val = KNOB_MAX_LED_VAL;
        uint16_t knob_max_val = 256;
        midi = midi_;
        m_leds = leds;

        knob_config knob_ctrl={
            Knob::knob_init_map{mux, mux->get(4), 6, knob_max_val, leds, knob_led_max_val, 0, 0}
        };

        button_config button_ctrl;

        init_internal(knob_ctrl, button_ctrl);
        select_instance(current_instance);
        LOG::LOG0("%s initialized %d\r\n", get_name(), current_instance);
    }

	Pwm *m_leds = nullptr;
	int last_led = PRESET_FIRST_ENC_LED;

    void select_mode(uint8_t index) {
        LOG::LOG0("%s %d special_function %d\r\n", get_name(), current_instance, index);
    }

    void force_mode(uint8_t index) {
        LOG::LOG0("%s %d forced %d\r\n", get_name(), current_instance, index);
    }

};

#endif /* SRC_BLOCKS_LED_DISP_H_ */
