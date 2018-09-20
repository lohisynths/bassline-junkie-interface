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

	typedef std::array<std::array<int, ADSR_COUNT>, ADSR_PARAM_NR> adsr_preset;

	~ADSR(){};

	virtual const char* get_name()
	{
	    return "ADSR";
	}

	enum ADSR_BUTTONS {
		ADSR0, ADSR1, ADSR2, LOOP
	};

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		uint8_t knob_led_count = COMMON_KNOB_LED_COUNT;
		uint8_t knob_val_max_val = KNOB_MAX_LED_VAL;
		uint16_t knob_max_val = 63;
		uint16_t button_val_max_val = KNOB_MAX_LED_VAL;

		knob_config knob_ctrl={
			Knob::knob_init_map{mux, mux->get(0), 9, knob_max_val, leds, knob_val_max_val, (ADSR_FIRST_ENC_LED + 30), knob_led_count},
			Knob::knob_init_map{mux, mux->get(0), 6, knob_max_val, leds, knob_val_max_val, (ADSR_FIRST_ENC_LED + 20), knob_led_count},
			Knob::knob_init_map{mux, mux->get(0), 3, knob_max_val, leds, knob_val_max_val, (ADSR_FIRST_ENC_LED + 10), knob_led_count},
			Knob::knob_init_map{mux, mux->get(0), 0, knob_max_val, leds, knob_val_max_val, (ADSR_FIRST_ENC_LED + 0),  knob_led_count},
		};

		button_config button_ctrl={
			Button::button_init_map{mux, mux->get(0), 15, leds, button_val_max_val, (ADSR_FIRST_BUTTON_LED+3)},
			Button::button_init_map{mux, mux->get(0), 14, leds, button_val_max_val, (ADSR_FIRST_BUTTON_LED+2)},
			Button::button_init_map{mux, mux->get(0), 13, leds, button_val_max_val, (ADSR_FIRST_BUTTON_LED+1)},
			Button::button_init_map{mux, mux->get(0), 12, leds, button_val_max_val, (ADSR_FIRST_BUTTON_LED+0)}
		};

		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void special_function(uint8_t index, uint8_t value) {
		DEBUG_LOG("%s %d special_function %d %d\r\n", get_name(), current_instance, index, value);
	}


	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

	uint8_t get_midi_nr(uint8_t index) {
		return ADSR_MIDI_OFFSET+index+(current_instance * ADSR_KNOB_COUNT);
	}

	void select_loop(uint8_t index, bool loop) {
		preset_values[ADSR_PARAM_NR-1][current_instance] ^= 1;
		auto &sw = get_sw();
		sw[index].set_led_val(preset_values[ADSR_PARAM_NR-1][current_instance] * sw_bright);

		DEBUG_LOG("%s %d ", get_name(), current_instance);

		bool LOOP = preset_values[ADSR_PARAM_NR-1][current_instance];
		if (LOOP) {
			DEBUG_LOG("LOOP ON\r\n");
		} else {
			DEBUG_LOG("LOOP OFF\r\n");
		}
	}

	adsr_preset &get_preset() {
		return preset_values;
	}

private:

};

#endif /* SRC_ADSR_H_ */
