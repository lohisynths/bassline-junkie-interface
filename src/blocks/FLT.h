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

#define FLT_MIDI_OFFSET				(33)

enum FLT_PARAMS {
	FLT_FREQ,
	FLT_RES,
	FLT_TYPE,
	FLT_PARAM_COUNT
};

class FLT : public UI_BLOCK<FLT_KNOB_COUNT, FLT_BUTTON_COUNT, FLT_PARAM_COUNT, FLT_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<FLT_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

	virtual const char* get_name() {
	    return "FLT";
	}

    uint8_t get_midi_ch(){
        return 1;
    }

	void select_function(uint8_t index) {
			select_filter_type(index);
	}

	void force_function(uint8_t index) {
			select_filter_type(index, true);
	}

	void select_filter_type(uint8_t index, bool force = 0) {
		if(last_filter_type != index || force) {
		    LOG::LOG0("%s type %d selected\r\n", get_name(), index);
			turn_off_sw(last_filter_type);
			turn_on_sw(index);
			set_current_preset_value(FLT_TYPE, index);
			last_filter_type = index;
            uint8_t midi_nr = get_current_instance_midi_nr(FLT_TYPE);
            get_midi()->send_cc(midi_nr, index, get_midi_ch());
		}
	}

    uint8_t get_midi_nr(uint8_t instance, uint8_t index) {
        return FLT_MIDI_OFFSET+index;
    }

private:
	uint8_t last_filter_type = 0;
};


#endif /* SRC_FLT_H_ */
