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
#define MOD_MIDI_CHANNEL            (2)

// MOD_COUNT = 6
// ADSR0, ADSR1, ADSR2, LFO0, LFO1, LFO2

// OSC0 pitch, OSC0 sin, OSC0 saw, OSC0 sqr, OSC0 rnd          = 5
// OSC1 pitch, OSC1 sin, OSC1 saw, OSC1 sqr, OSC1 rnd          = 5
// OSC2 pitch, OSC2 sin, OSC2 saw, OSC2 sqr, OSC2 rnd          = 5
// FLT freq, FLT reso                                          = 2
//                                                               17
#define MOD_PARAM_COUNT 17

class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_PARAM_COUNT, MOD_COUNT> {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MOD_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

    void init(knob_config knob_ctrl, button_config button_ctrl) {
        init_internal(knob_ctrl, button_ctrl);
        select_instance(current_instance);
    }

    virtual ~MOD(){};

	virtual const char* get_name() {
	    return "MOD";
	}

    virtual int16_t get_current_preset_value(const uint8_t index) {
        return preset_values[current_instance][actual_mod_dest];
    }

    virtual void set_current_preset_value(const uint8_t index, const uint16_t value) {
        preset_values[current_instance][actual_mod_dest] = value;
    }

    virtual void select_MOD_dest(const int index) {
        LOG::LOG0("%s dst %d selected\r\n", get_name(), index);
        actual_mod_dest = index;
    }

	virtual uint8_t get_midi_ch(){
        return MOD_MIDI_CHANNEL;
    }

	virtual uint8_t get_midi_nr(const uint8_t instance, const uint8_t index) {
        return instance + (index*MOD_COUNT);
    }

	virtual uint8_t get_current_instance_midi_nr(const uint8_t index) {
        LOG::LOG0("%s midi index %d\r\n", get_name(), index);
        return get_midi_nr(current_instance, actual_mod_dest);
    }

    virtual void select_mode(const uint8_t index) { }
    virtual void force_mode(const uint8_t index) { }
    virtual void knob_sw_changed(const uint8_t index, bool state) { }

private:
	uint8_t actual_mod_dest = 0;
};

#endif /* SRC_MOD_H_ */
