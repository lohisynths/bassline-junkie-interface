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
#define ADSR_FIRST_ENC_MUX_ADR		(0)

#define ADSR_FIRST_BUTTON_LED		(ADSR_KNOB_COUNT*10)
#define ADSR_FIRST_BUTTON_MUX_ADR	(ADSR_KNOB_COUNT*ADSR_COUNT)


#define ADSR_KNOB_COUNT				(4)
#define ADSR_BUTTON_COUNT			(4)
#define ADSR_COUNT					(3)

#define ADSR_MIDI_OFFSET			(18)

// ADSR_KNOB_COUNT+1 (1 = loop)
#define ADSR_PARAM_NR (ADSR_KNOB_COUNT+1)

class ADSR : public UI_BLOCK<ADSR_KNOB_COUNT, ADSR_BUTTON_COUNT, ADSR_COUNT> {
public:

	typedef std::array<std::array<int, ADSR_PARAM_NR>, ADSR_COUNT> adsr_preset;
	adsr_preset knobs_values = {0};

	~ADSR(){};

	char const *NAME = "ADSR";
	enum ADSR_BUTTONS {
		ADSR0, ADSR1, ADSR2, LOOP
	};

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data adsr_ctl[ADSR_KNOB_COUNT] = {
				ADSR_FIRST_ENC_LED + 30, ADSR_FIRST_ENC_MUX_ADR + 9, mux->get(0),
				ADSR_FIRST_ENC_LED + 20, ADSR_FIRST_ENC_MUX_ADR + 6, mux->get(0),
				ADSR_FIRST_ENC_LED + 10, ADSR_FIRST_ENC_MUX_ADR + 3, mux->get(0),
				ADSR_FIRST_ENC_LED +  0, ADSR_FIRST_ENC_MUX_ADR + 0, mux->get(0)
		};
		sw_data adsr_ctl_sw[ADSR_BUTTON_COUNT] = {
				ADSR_FIRST_BUTTON_LED+3, ADSR_FIRST_BUTTON_MUX_ADR + 3, mux->get(0),
				ADSR_FIRST_BUTTON_LED+2, ADSR_FIRST_BUTTON_MUX_ADR + 2, mux->get(0),
				ADSR_FIRST_BUTTON_LED+1, ADSR_FIRST_BUTTON_MUX_ADR + 1, mux->get(0),
				ADSR_FIRST_BUTTON_LED,   ADSR_FIRST_BUTTON_MUX_ADR,     mux->get(0)
		};
		init_internal(*leds, adsr_ctl, adsr_ctl_sw);
		Button *sw = get_sw();
		sw[current_instance].set_led_val(sw_bright);
	}

	virtual void button_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d button switch %d", NAME, current_instance, index);
		if (state)
			DEBUG_LOG(" pushed\r\n");
		else
			DEBUG_LOG(" released\r\n");

		if (state) {
			if (index == LOOP) {
				select_loop(index, state);
			} else {
				if (index != current_instance) {
					select_adsr(index);
				}
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d encoder switch %d ", NAME, current_instance, index);
		if (state)
			DEBUG_LOG("pushed\r\n");
		else
			DEBUG_LOG("released\r\n");
	}

	virtual void knob_val_changed(uint8_t index, int value) {
		DEBUG_LOG("%s %d value %d changed %d\r\n", NAME, current_instance, index, value);

		Knob *knob = get_knobs();
		int16_t actual_value = knob[index].get_value();
		knobs_values[current_instance][index] = actual_value;

		int led_nr = actual_value / 7;
		knob[index].led_on(led_nr, led_bright);

		// attack decay sustain release - one knob per function
		int16_t tmp = (value*2);
		if(tmp < 1)
			tmp=1;
		if(tmp > 127)
			tmp = 127;

		midi->send_cc(ADSR_MIDI_OFFSET+index+(current_instance * ADSR_KNOB_COUNT), tmp, 1);
	}

	void select_adsr(uint8_t index) {
		Button *sw = get_sw();

		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current adsr and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < ADSR_KNOB_COUNT; i++) {
			Knob *knob = get_knobs();
			knob[i].set_value(knobs_values[current_instance][i]);

			int led_nr = knob[i].get_value() / 7;
			knob[i].led_on(led_nr, led_bright);
		}

		sw[LOOP].set_led_val(knobs_values[current_instance][ADSR_PARAM_NR-1] * sw_bright);

		DEBUG_LOG("%s %d SELECTED\r\n", NAME, index);
	};

	void select_loop(uint8_t index, bool loop) {
		knobs_values[current_instance][ADSR_PARAM_NR-1] ^= 1;
		Button *sw = get_sw();
		sw[index].set_led_val(knobs_values[current_instance][ADSR_PARAM_NR-1] * sw_bright);

		DEBUG_LOG("%s %d ", NAME, current_instance);
		if (knobs_values[current_instance][ADSR_PARAM_NR-1]) {
			DEBUG_LOG("LOOP ON\r\n");
		} else {
			DEBUG_LOG("LOOP OFF\r\n");
		}
	}



	void set_preset(adsr_preset input) {

		for (int i = 0; i < ADSR_COUNT; i++) {
			for (int j = 0; j < ADSR_PARAM_NR; j++) {
					knobs_values[i][j] = input[i][j];
			}
		}
		select_adsr(current_instance);
	};

	adsr_preset &get_preset() {
		return knobs_values;
	}

private:
	int led_bright = 256;
	int sw_bright = 1024;
	uint8_t current_instance = 0;
	MIDI *midi;

};

#endif /* SRC_ADSR_H_ */
