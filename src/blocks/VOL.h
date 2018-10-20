/*
 * VOL.h
 *
 *  Created on: Jun 9, 2018
 *      Author: alax
 */

#ifndef SRC_BLOCKS_VOL_H_
#define SRC_BLOCKS_VOL_H_

#include "utils.h"

#include "Knob.h"
#include "Button.h"

#define VOL_KNOB_COUNT				(1)
#define VOL_BUTTON_COUNT			(0)
#define VOL_COUNT					(1)

#define VOL_PARAM_COUNT			(VOL_KNOB_COUNT)
#define VOL_MIDI_OFFSET             126

class VOL : public UI_BLOCK<VOL_KNOB_COUNT, VOL_BUTTON_COUNT, VOL_PARAM_COUNT, VOL_COUNT> {
public:

    using UI_BLOCK::UI_BLOCK;

    bool if_knob_sw_pushed(){
	    auto &knobs = get_knobs();
	    return !knobs[0].get_switch_state();
	}
	virtual const char* get_name() {
	    return "VOL";
	}

    uint8_t get_midi_ch(){
        return 0;
    }

    uint8_t get_midi_nr(uint8_t instance, uint8_t index) {
        return VOL_MIDI_OFFSET;
    }

	void button_changed(uint8_t index, bool state) {};

	void select_instance(uint8_t index) {};

	uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(get_current_instasnce(), index);
	}

    void select_function(uint8_t index) {
        LOG::LOG0("%s %d special_function %d\r\n", get_name(), get_current_instasnce(), index);
    }

    void force_function(uint8_t index) {
        LOG::LOG0("%s %d forced %d\r\n", get_name(), get_current_instasnce(), index);
    }

    uint8_t get_actual_preset_nr() {
        return actual_preset_value;
    }

    uint8_t actual_preset_value = 0;
};

#endif /* SRC_BLOCKS_VOL_H_ */
