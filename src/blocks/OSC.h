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

	void init(knob_config knob_ctrl, button_config button_ctrl) {
		init_internal(knob_ctrl, button_ctrl);
		select_instance(current_instance);
	}

	void select_mode(uint8_t index) {
	    LOG::LOG0("%s %d special_function %d\r\n", get_name(), current_instance, index);
	}

	void force_mode(uint8_t index) {
	    LOG::LOG0("%s %d forced %d\r\n", get_name(), current_instance, index);
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {

	}

   uint8_t get_midi_nr(uint8_t instance, uint8_t index){
        return OSC_MIDI_OFFSET+index+(instance * (OSC_KNOB_COUNT));
    }

   uint8_t get_current_instance_midi_nr(uint8_t index) {
       return get_midi_nr(current_instance, index);
   }

	uint8_t get_current_osc() { return current_instance; };

private:

};


#endif /* SRC_OSC_H_ */
