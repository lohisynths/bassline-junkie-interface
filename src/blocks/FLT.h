/*
 * FLT.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_FLT_H_
#define SRC_FLT_H_

#include "UI_BLOCK.h"

#define FLT_KNOB_COUNT				(2)
#define FLT_BUTTON_COUNT			(3)
#define FLT_COUNT					(1)

#define FLT_FIRST_ENC_LED			(9*16)

#define FLT_FIRST_BUTTON_LED		(FLT_FIRST_ENC_LED+28)

#define FLT_MIDI_OFFSET				(32)


class FLT : public UI_BLOCK<FLT_KNOB_COUNT, FLT_BUTTON_COUNT, FLT_COUNT> {
public:
	~FLT(){};

	char const *NAME = "FLT";

	void init_internal(Pwm &leds, knob_data knobdata[FLT_KNOB_COUNT], sw_data swdata[FLT_BUTTON_COUNT]) {
		for (int i = 0; i < FLT_KNOB_COUNT; i++) {
			Knob *knob = get_knobs();
			if(i == 0) {
				knob[i].init(knobdata[i].knobs_first_led, knobdata[i].knobs_first_mux_adr, leds, *knobdata[i].knobs_mux_data, 119, 12);
				// cutoff frequency knob
			} else {
				knob[i].init(knobdata[i].knobs_first_led, knobdata[i].knobs_first_mux_adr, leds, *knobdata[i].knobs_mux_data, 64, 10);
			}

		}
		for (int i = 0; i < FLT_BUTTON_COUNT; i++) {
			Button *sw = get_sw();
			sw[i].init(swdata[i].sw_first_led,  swdata[i].sw_first_mux_adr, leds, *swdata[i].sw_mux_data);
		}
	};

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data FLT_ctl[FLT_KNOB_COUNT] = {
				FLT_FIRST_ENC_LED + 0 , 4,  mux->get(3),
				FLT_FIRST_ENC_LED + 16, 7,  mux->get(3)
		};

		sw_data FLT_ctl_sw[FLT_BUTTON_COUNT] = {
				FLT_FIRST_BUTTON_LED+2, 15, mux->get(3),
				FLT_FIRST_BUTTON_LED+1, 14,	mux->get(3),
				FLT_FIRST_BUTTON_LED,   13,	mux->get(3),

		};
		init_internal(*leds, FLT_ctl, FLT_ctl_sw);
		Button *sw = get_sw();
		sw[FLT_mode].set_led_val(sw_bright);
	}

	virtual void button_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s button switch %d", NAME, index);
		if (state)
			DEBUG_LOG(" pushed\r\n");
		else
			DEBUG_LOG(" released\r\n");

		if (state) {
			if (index != FLT_mode) {
				select_MODE(index);
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {
		DEBUG_LOG("%s encoder switch %d ", NAME, index);
		if (state)
			DEBUG_LOG("pushed\r\n");
		else
			DEBUG_LOG("released\r\n");
	}

	virtual void knob_val_changed(uint8_t index, int value) {
		DEBUG_LOG("%s value %d changed %d\r\n", NAME, index, value);

		Knob *knob = get_knobs();
		int16_t actual_value = knob[index].get_value();


		if(index == 0) {
			int led_nr = actual_value / 10;
			knob[index].led_on(led_nr, led_bright);
			// resonance cutoff - one knob per function
			midi->send_cc(FLT_MIDI_OFFSET+index, value, 0);
			// cutoff frequency knob
		} else {
			int led_nr = actual_value / 7;
			knob[index].led_on(led_nr, led_bright);
			// resonance cutoff - one knob per function
			midi->send_cc(FLT_MIDI_OFFSET+index, value*2, 0);
		}





	}

	void select_MODE(uint8_t index) {
		Button *sw = get_sw();

		sw[index].set_led_val(sw_bright);

		// get button number of button from current FLT and turn led off
		sw[FLT_mode].set_led_val(0);

		FLT_mode = index;

		DEBUG_LOG("%s %d SELECTED\r\n", NAME, index);
	};

private:
	int16_t FLT_mode={};

	int led_bright = 256;
	int sw_bright = 1024;
	MIDI *midi;

};


#endif /* SRC_FLT_H_ */
