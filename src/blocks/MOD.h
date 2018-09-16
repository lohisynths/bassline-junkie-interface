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

#define MOD_MIDI_OFFSET 			(96)

#define MOD_PARAM_COUNT 			(MOD_KNOB_COUNT)

class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_PARAM_COUNT, MOD_COUNT> {
public:
	~MOD(){};

	virtual const char* get_name()
	{
	    return "MOD";
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data MOD_ctl[MOD_KNOB_COUNT] = {
				MOD_FIRST_ENC_LED +  0, 0,  mux->get(2)
		};

		sw_data MOD_ctl_sw[MOD_BUTTON_COUNT] = {
				MOD_FIRST_BUTTON_LED+5, 8,  mux->get(2),
				MOD_FIRST_BUTTON_LED+4, 7,	mux->get(2),
				MOD_FIRST_BUTTON_LED+3, 6, 	mux->get(2),
				MOD_FIRST_BUTTON_LED+2, 5,  mux->get(2),
				MOD_FIRST_BUTTON_LED+1, 4,	mux->get(2),
				MOD_FIRST_BUTTON_LED,   3, 	mux->get(2)

		};
		init_internal(*leds, MOD_ctl, MOD_ctl_sw);
		select_instance(current_instance);
	}

	virtual void button_changed(uint8_t index, bool state) {
		if (state) {
			if (index != current_instance) {
				select_instance(index);
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

	virtual void knob_val_changed(uint8_t index, uint16_t value_scaled) {
		knob[index].set_leds(value_scaled);

		int midi_nr = current_instance + (mod_dest*MOD_COUNT);
		midi->send_cc(midi_nr, (value_scaled), 2);
	}

	void select_instance(uint8_t index) {
		auto &sw = get_sw();

		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current MOD and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < MOD_KNOB_COUNT; i++) {
			auto &knob = get_knobs();
			knob[i].set_value(knob_values[i][current_instance]);

			int led_nr = knob[i].get_value();
			led_nr = led_nr/ 7;
			knob[i].led_on(led_nr, led_bright);
		}

		DEBUG_LOG("%s %d SELECTED\r\n", get_name(), index);
	};

	void select_MOD_dest(int index) {
		if(index > -1) {
			mod_dest = index;
		}
	}

private:

	MIDI *midi;

	uint8_t mod_dest = 0;

};


#endif /* SRC_MOD_H_ */
