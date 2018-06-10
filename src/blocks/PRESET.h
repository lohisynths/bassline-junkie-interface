/*
 * PRESET.h
 *
 *  Created on: Jun 9, 2018
 *      Author: alax
 */

#ifndef SRC_BLOCKS_PRESET_H_
#define SRC_BLOCKS_PRESET_H_

#include "../utils.h"

#include "../Knob.h"
#include "../Button.h"

#define PRESET_KNOB_COUNT				(1)
#define PRESET_BUTTON_COUNT				(0)
#define PRESET_COUNT					(1)

#define PRESET_FIRST_ENC_LED			(11*16)
#define SEGMENTS						(8)

class PRESET : public UI_BLOCK<PRESET_KNOB_COUNT, PRESET_BUTTON_COUNT, PRESET_COUNT> {
public:
	PRESET(){};
	virtual ~PRESET(){};

	char const *NAME = "PRESET";

	void button_changed(uint8_t index, bool state) {};

	void init_internal(Pwm &leds, knob_data knobdata[PRESET_KNOB_COUNT], sw_data swdata[PRESET_BUTTON_COUNT]) {
		for (int i = 0; i < PRESET_KNOB_COUNT; i++) {
			Knob *knob = get_knobs();
			knob[i].init(knobdata[i].knobs_first_led, knobdata[i].knobs_first_mux_adr, leds, *knobdata[i].knobs_mux_data, 127, 10);
		}
		for (int i = 0; i < PRESET_BUTTON_COUNT; i++) {
			Button *sw = get_sw();
			sw[i].init(swdata[i].sw_first_led,  swdata[i].sw_first_mux_adr, leds, *swdata[i].sw_mux_data);
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d encoder switch %d ", NAME, current_instance, index);
		if (state)
			DEBUG_LOG("pushed\r\n");
		else
			DEBUG_LOG("released\r\n");
	}

	void knob_val_changed(uint8_t index, int value) {
		uint8_t first_nr  = value % 10;
		uint8_t second_nr = (value / 10) % 10;
		uint8_t third_nr  = value / 100;

		DEBUG_LOG("%s %d %d %d\r\n", NAME,  third_nr, second_nr, first_nr);

		//set_all(first_nr*128);

		set_digit(2, first_nr);
		set_digit(1, second_nr);
		set_digit(0, third_nr);

		//DEBUG_LOG("%s %d value %d changed %d\r\n", NAME, current_instance, index, value);
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

		for(int i=0;i<7;i++) {
			m_leds->set(PRESET_FIRST_ENC_LED+i+(seg_nr*SEGMENTS), digits[digit][i]*led_bright);
		}

//		m_leds->set(PRESET_FIRST_ENC_LED+digit, 1024);
//		m_leds->set(last_led, 0);
//
//		last_led = PRESET_FIRST_ENC_LED+digit;
	}

	void set_all(uint16_t val) {
		for(int i=0;i<SEGMENTS*3;i++) {
			m_leds->set(PRESET_FIRST_ENC_LED+i, val);
		}
	}


	void init(Mux *mux, Pwm *leds, MIDI *midi) {


		m_mux = mux;
		m_leds = leds;
		m_midi = midi;

		knob_data PRESET_ctl[PRESET_KNOB_COUNT] = {0, 6, mux->get(4) };

		sw_data PRESET_ctl_sw[PRESET_BUTTON_COUNT];// = {

		init_internal(*leds, PRESET_ctl, PRESET_ctl_sw);



	//	knob.init(led_index, mux_index, *m_leds, *m_mux->get(2), max_val, leds_count);



	}
	int last_led = PRESET_FIRST_ENC_LED;

	int led_bright = 1024;
	uint8_t current_instance = 0;
	MIDI *m_midi;
	Pwm *m_leds;
	Mux *m_mux;
	Knob knob;


};

#endif /* SRC_BLOCKS_PRESET_H_ */
