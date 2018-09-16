/*
 * LFO.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_LFO_H_
#define SRC_LFO_H_

#include "UI_BLOCK.h"

#define LFO_KNOB_COUNT				(1)
#define LFO_BUTTON_COUNT			(8)
#define LFO_COUNT					(3)

#define LFO_FIRST_ENC_LED			(8*16)

#define LFO_FIRST_BUTTON_LED		(LFO_FIRST_ENC_LED+10)

#define LFO_MIDI_OFFSET				(36)

enum LFO_PARAMS {
	LFO_FREQ,
	LFO_PARAM_COUNT
};


class LFO : public UI_BLOCK<LFO_KNOB_COUNT, LFO_BUTTON_COUNT, LFO_PARAM_COUNT, LFO_COUNT> {
public:
	~LFO(){};

	virtual const char* get_name()
	{
	    return "LFO";
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data LFO_ctl[LFO_KNOB_COUNT] = {
				LFO_FIRST_ENC_LED +  0, 0,  mux->get(4)
		};

		sw_data LFO_ctl_sw[LFO_BUTTON_COUNT] = {
				LFO_FIRST_BUTTON_LED+20, 0,  mux->get(3),
				LFO_FIRST_BUTTON_LED+19, 15, mux->get(2),
				LFO_FIRST_BUTTON_LED+18, 14, mux->get(2),

				LFO_FIRST_BUTTON_LED+4, 13,  mux->get(2),
				LFO_FIRST_BUTTON_LED+3, 12,  mux->get(2),
				LFO_FIRST_BUTTON_LED+2, 11,  mux->get(2),
				LFO_FIRST_BUTTON_LED+1, 10,	 mux->get(2),
				LFO_FIRST_BUTTON_LED,   9, 	 mux->get(2),
		};
		init_internal(*leds, LFO_ctl, LFO_ctl_sw);
		select_instance(current_instance);
	}

	virtual void button_changed(uint8_t index, bool state) {
		if (state) {
			if (index < LFO_COUNT) {
				if (index != current_instance) {
					select_LFO(index);
				}
			} else {
				select_shape(index-LFO_COUNT);
			}
		}
	};

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

	virtual void knob_val_changed(uint8_t index, uint16_t value_scaled) {
		knob[index].set_leds(value_scaled);
		knob_values[current_instance][index] = value_scaled;

		/*
		LFO_OFFSET 36	0 + LFO_PARAMS*0	36	LFO 0 SHAPE
			1 + LFO_PARAMS*0	37	LFO 0 FREQ
			0 + LFO_PARAMS*1	38	LFO 1 SHAPE
			1 + LFO_PARAMS*1	39	LFO 1 FREQ
			0 + LFO_PARAMS*2	40	LFO 2 SHAPE
			1 + LFO_PARAMS*2	41	LFO 2 FREQ
		*/
		int midi_nr = LFO_MIDI_OFFSET+index+ 1 + (current_instance*2);
		midi->send_cc(midi_nr, value_scaled, 1);
	}

	void select_LFO(uint8_t index) {
		auto &sw = get_sw();

		//DEBUG_LOG("%s %d lase SHAPE %d\r\n", NAME, current_instance, LFO_shape[current_instance]);
		// LFO_shape[current_instance] + LFO_COUNT
		// LFO_COUNT -> offset in midi mapping
		uint8_t mod_button_led = LFO_shape[current_instance] + LFO_COUNT;
		sw[mod_button_led].set_led_val(0);
		mod_button_led = LFO_shape[index] + LFO_COUNT;
		sw[mod_button_led].set_led_val(sw_bright);
		sw[index].set_led_val(sw_bright);

		// get button number of button from current LFO and turn led off
		sw[current_instance].set_led_val(0);

		current_instance = index;

		for (int i = 0; i < LFO_KNOB_COUNT; i++) {
			auto &knob = get_knobs();
			knob[i].set_value(knob_values[i][current_instance]);

			int led_nr = knob[i].get_value() / 7;
			knob[i].led_on(led_nr, led_bright);
		}

		DEBUG_LOG("%s %d SELECTED, SHAPE %d\r\n", get_name(), index, LFO_shape[current_instance]);
	};

	void select_shape(uint8_t index) {
		auto &sw = get_sw();
		sw[index+LFO_COUNT].set_led_val(sw_bright);
		// get button number of button from current LFO and turn led off
		sw[LFO_shape[current_instance]+LFO_COUNT].set_led_val(0);

		if(LFO_shape[current_instance] != index) {

			LFO_shape[current_instance] = index;
			DEBUG_LOG("%s %d SHAPE %d\r\n", get_name(), current_instance, index);

			/*
			LFO_OFFSET 36
				0 + LFO_PARAMS*0	36	LFO 0 SHAPE
				1 + LFO_PARAMS*0	37	LFO 0 FREQ
				0 + LFO_PARAMS*1	38	LFO 1 SHAPE
				1 + LFO_PARAMS*1	39	LFO 1 FREQ
				0 + LFO_PARAMS*2	40	LFO 2 SHAPE
				1 + LFO_PARAMS*2	41	LFO 2 FREQ
			*/
			// controll only frequency of every LFO
			midi->send_cc(LFO_MIDI_OFFSET + (current_instance*2), index*24, 1);
		}
	}

	void select_instance(uint8_t index) {
		for (int i = 0; i < LFO_KNOB_COUNT; i++) {
				auto &knob = get_knobs();
				knob[i].set_value(knob_values[0][i]);
				knob_val_changed(i, knob_values[0][i]);
		}
		DEBUG_LOG("%s %d SELECTED\r\n", get_name(), index);
	};

private:
	int16_t LFO_shape[LFO_COUNT]={};

	uint8_t current_instance = 0;
	MIDI *midi;
};


#endif /* SRC_LFO_H_ */
