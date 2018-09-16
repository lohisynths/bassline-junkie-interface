/*
 * OSC.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_OSC_H_
#define SRC_OSC_H_

#include "UI_BLOCK.h"

#define OSC_KNOB_COUNT				(5)
#define OSC_BUTTON_COUNT			(3)
#define OSC_COUNT					(3)

#define OSC_FIRST_ENC_LED			(3*16)

#define OSC_FIRST_BUTTON_LED		(6*16+(12))

#define OSC_MIDI_OFFSET 			(1)

enum OSC_PARAMS {
	OSC_PITCH,
	OSC_SIN,
	OSC_SAW,
	OSC_SQR,
	OSC_RND,
	OSC_PARAM_COUNT
};

class OSC : public UI_BLOCK<OSC_KNOB_COUNT, OSC_BUTTON_COUNT, OSC_PARAM_COUNT, OSC_COUNT> {
public:
	typedef std::array<std::array<int, OSC_COUNT>, OSC_PARAM_COUNT> osc_preset;
	~OSC(){};

	virtual const char* get_name()
	{
	    return "OSC";
	}

	void init(Mux *mux, Pwm *leds, MIDI *midi_) {
		midi = midi_;
		knob_data OSC_ctl[OSC_KNOB_COUNT] = {
				OSC_FIRST_ENC_LED + 48, 12, mux->get(1),
				OSC_FIRST_ENC_LED + 30, 9,  mux->get(1),
				OSC_FIRST_ENC_LED + 20, 6,  mux->get(1),
				OSC_FIRST_ENC_LED + 10, 3,  mux->get(1),
				OSC_FIRST_ENC_LED +  0, 0,  mux->get(1)
		};

		sw_data OSC_ctl_sw[OSC_BUTTON_COUNT] = {
				OSC_FIRST_BUTTON_LED+2, 3, 	  mux->get(3),
				OSC_FIRST_BUTTON_LED+1, 2,	  mux->get(3),
				OSC_FIRST_BUTTON_LED,   15,   mux->get(1)
		};
		init_internal(*leds, OSC_ctl, OSC_ctl_sw);
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

	uint8_t get_midi_nr(uint8_t index) {
		return OSC_MIDI_OFFSET+index+(current_instance * (OSC_KNOB_COUNT+1));
	}

	void select_instance(uint8_t index) {
		auto &sw = get_sw();
		sw[index].set_led_val(sw_bright);

		if(index != current_instance) {
			// get button number of button from current OSC and turn led off
			sw[current_instance].set_led_val(0);
			current_instance = index;
		}

		for (int i = 0; i < OSC_KNOB_COUNT; i++) {
			auto &knob = get_knobs();
			uint8_t val = knob_values[i][current_instance];
			knob[i].set_value(val);
			knob[i].set_leds(val);
		}

		DEBUG_LOG("%s %d SELECTED\r\n", get_name(), index);
	};

	uint8_t get_current_osc() { return current_instance; };

	void set_preset(osc_preset input) {
		memcpy(&knob_values, &input, sizeof(input));
		select_instance(current_instance);
	};

	osc_preset &get_preset() {
		return knob_values;
	}

private:

};


#endif /* SRC_OSC_H_ */
