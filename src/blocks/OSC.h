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

#define OSC_MIDI_OFFSET 			(0)


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

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<OSC_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

	virtual const char* get_name() {
	    return "OSC";
	}

    uint8_t get_midi_ch(){
        return 1;
    }

	void select_function(uint8_t index) {
	    LOG::LOG0("%s %d special_function %d\r\n", get_name(), get_current_instasnce(), index);
	}

	void force_function(uint8_t index) {
	    LOG::LOG0("%s %d forced %d\r\n", get_name(), get_current_instasnce(), index);
	}

   uint8_t get_midi_nr(uint8_t instance, uint8_t index){
        return OSC_MIDI_OFFSET+index+(instance * (OSC_KNOB_COUNT));
    }

   uint8_t get_current_instance_midi_nr(uint8_t index) {
       return get_midi_nr(get_current_instasnce(), index);
   }

	uint8_t get_current_osc() { return get_current_instasnce(); };

private:

};


#endif /* SRC_OSC_H_ */
