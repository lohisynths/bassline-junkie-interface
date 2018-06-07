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


class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_COUNT> {
public:
	~MOD(){};

	char const *NAME = "MOD";

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data MOD_ctl[MOD_KNOB_COUNT] = {
				MOD_FIRST_ENC_LED +  0, 0,  mux->get(2)
		};

		sw_data MOD_ctl_sw[MOD_BUTTON_COUNT] = {
				MOD_FIRST_BUTTON_LED,   3, 	mux->get(2),
				MOD_FIRST_BUTTON_LED+1, 4,	mux->get(2),
				MOD_FIRST_BUTTON_LED+2, 5,  mux->get(2),
				MOD_FIRST_BUTTON_LED+3, 6, 	mux->get(2),
				MOD_FIRST_BUTTON_LED+4, 7,	mux->get(2),
				MOD_FIRST_BUTTON_LED+5, 8,  mux->get(2)
		};
		init_internal(*leds, MOD_ctl, MOD_ctl_sw);
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
			if (index != current_instance) {
				select_MOD(index);
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
		MOD_val[current_instance][index] = actual_value;

		int led_nr = actual_value / 7;
		knob[index].led_on(led_nr, led_bright);
	}

	void select_MOD(uint8_t index) {
		Button *sw = get_sw();

		sw[index].set_led_val(sw_bright);

		// get button number of button from current MOD and turn led off
		sw[current_instance].set_led_val(0);

		current_instance = index;

		for (int i = 0; i < MOD_KNOB_COUNT; i++) {
			Knob *knob = get_knobs();
			knob[i].set_value(MOD_val[current_instance][i]);

			int led_nr = knob[i].get_value() / 7;
			knob[i].led_on(led_nr, led_bright);
		}

		DEBUG_LOG("%s %d SELECTED\r\n", NAME, index);
	};

private:
	int16_t MOD_val[MOD_COUNT][MOD_KNOB_COUNT]={};

	int led_bright = 256;
	int sw_bright = 1024;
	uint8_t current_instance = 0;
	MIDI *midi;


};


#endif /* SRC_MOD_H_ */
