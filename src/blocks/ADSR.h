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

	typedef std::array<std::array<int, ADSR_COUNT>, ADSR_PARAM_NR> adsr_preset;

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
		auto &sw = get_sw();
		sw[current_instance].set_led_val(sw_bright);
	}

	virtual void button_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d button switch %d", NAME, current_instance, index);
		DEBUG_LOG( (state) ? " pushed\r\n" : " released\r\n" );

		if (state) {
			if (index == LOOP) {
				select_loop(index, state);
			} else {
				if (index != current_instance) {
					select_instance(index);
				}
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s %d encoder switch %d ", NAME, current_instance, index);
		DEBUG_LOG( (state) ? " pushed\r\n" : " released\r\n" );

	}

	virtual void knob_val_changed(uint8_t index) {
		auto &knob = get_knobs();
		int16_t value_scaled = knob[index].get_value_scaled();
		DEBUG_LOG("%s %d value %d changed %d\r\n", NAME, current_instance, index, value_scaled);

		knob[index].set_leds(value_scaled);
		knob_values[current_instance][index] = value_scaled;

		midi->send_cc(ADSR_MIDI_OFFSET+index+(current_instance * ADSR_KNOB_COUNT), value_scaled, 1);
	}

	void select_instance(uint8_t index) {
		auto &sw = get_sw();
		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current adsr and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < ADSR_KNOB_COUNT; i++) {
			auto &knob = get_knobs();
			uint8_t val = knob_values[i][current_instance];
			knob[i].set_value(val);
			knob[i].set_leds(val);
		}
		sw[LOOP].set_led_val(knob_values[ADSR_PARAM_NR-1][current_instance] * sw_bright);
		DEBUG_LOG("%s %d SELECTED\r\n", NAME, index);
	};

	void select_loop(uint8_t index, bool loop) {
		knob_values[ADSR_PARAM_NR-1][current_instance] ^= 1;
		auto &sw = get_sw();
		sw[index].set_led_val(knob_values[current_instance][ADSR_PARAM_NR-1] * sw_bright);

		DEBUG_LOG("%s %d ", NAME, current_instance);

		bool LOOP = knob_values[current_instance][ADSR_PARAM_NR-1];
		if (LOOP) {
			DEBUG_LOG("LOOP ON\r\n");
		} else {
			DEBUG_LOG("LOOP OFF\r\n");
		}
	}

	adsr_preset &get_preset() {
		return knob_values;
	}

private:
	int led_bright = 256;
	int sw_bright = 1024;
	MIDI *midi;
};

#endif /* SRC_ADSR_H_ */
