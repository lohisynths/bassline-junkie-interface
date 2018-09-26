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
	LFO_SHAPE,
	LFO_PARAM_COUNT
};
/*
LFO_OFFSET 36	0 + LFO_PARAMS*0	36	LFO 0 SHAPE
	1 + LFO_PARAMS*0	37	LFO 0 FREQ
	0 + LFO_PARAMS*1	38	LFO 1 SHAPE
	1 + LFO_PARAMS*1	39	LFO 1 FREQ
	0 + LFO_PARAMS*2	40	LFO 2 SHAPE
	1 + LFO_PARAMS*2	41	LFO 2 FREQ
*/
/*
LFO_OFFSET 36
	0 + LFO_PARAMS*0	36	LFO 0 SHAPE
	1 + LFO_PARAMS*0	37	LFO 0 FREQ
	0 + LFO_PARAMS*1	38	LFO 1 SHAPE
	1 + LFO_PARAMS*1	39	LFO 1 FREQ
	0 + LFO_PARAMS*2	40	LFO 2 SHAPE
	1 + LFO_PARAMS*2	41	LFO 2 FREQ
*/

class LFO : public UI_BLOCK<LFO_KNOB_COUNT, LFO_BUTTON_COUNT, LFO_PARAM_COUNT, LFO_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<LFO_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

	virtual const char* get_name() {
	    return "LFO";
	}

    uint8_t get_midi_ch(){
        return 1;
    }

	void select_lfo_shape(uint8_t index, bool force = 0) {
		if(last_lfo_shape != index || force) {
		    LOG::LOG0("%s mod %d selected\r\n", get_name(), index);

			uint8_t shape_button_adr = index + LFO_COUNT;
			uint8_t last_shape_button_adr = last_lfo_shape + LFO_COUNT;

			turn_off_sw(last_shape_button_adr);
			turn_on_sw(shape_button_adr);

			set_current_preset_value(LFO_SHAPE, index);
			last_lfo_shape = index;
			uint8_t midi_nr = get_current_instance_midi_nr(LFO_SHAPE);
	        get_midi()->send_cc(midi_nr, index, get_midi_ch());
		}
	}

	void select_function(uint8_t index) {
		select_lfo_shape(index);
	}

	void force_function(uint8_t index) {
	    LOG::LOG0("%s mode %d forced\r\n", get_name(), index);
		select_lfo_shape(index, true);
	}

    uint8_t get_midi_nr(uint8_t instance, uint8_t index){
        return LFO_MIDI_OFFSET+index + (instance*2);
    }

    uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(get_current_instasnce(), index);
    }

private:
	uint8_t last_lfo_shape = 0;
};


#endif /* SRC_LFO_H_ */
