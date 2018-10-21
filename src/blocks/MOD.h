/*
 * MOD.h
 *
 *  Created on: Jun 5, 2018
 *      Author: alax
 */

#ifndef SRC_MOD_H_
#define SRC_MOD_H_

#include "UI_BLOCK.h"
#include "VOL.h"

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
#define MOD_PARAM_COUNT 0

class MOD : public UI_BLOCK<MOD_KNOB_COUNT, MOD_BUTTON_COUNT, MOD_PARAM_COUNT, MOD_COUNT> {
public:

    void init_(ADSR *adsr, OSC *osc, LFO *lfo, MOD *mod, FLT *filter, VOL *vol) {
        _adsr = adsr;
        _osc = osc;
        _lfo = lfo;
        _mod = mod;
        _filter = filter;
        _vol = vol;
    }

    void update2() {
        int ret = _osc->get_first_knob_sw_pushed();
        if(ret > -1) {
            select_MOD_dest(ret+(_osc->get_current_osc()*5));
        }

        ret = _filter->get_first_knob_sw_pushed();
        if(ret > -1) {
            select_MOD_dest(ret+15);
        }

        ret = get_first_knob_sw_pushed();
        if(ret > -1) {
            mod_viever_mode ^= 1;
            _osc->set_viewer_mode(mod_viever_mode);
            LOG::LOG1("mod_viever mode changed %d\r\n", mod_viever_mode);
        }

    }
    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MOD_LOG_LEVEL> LOG;

    using UI_BLOCK::UI_BLOCK;

    virtual ~MOD(){};

	virtual const char* get_name() {
	    return "MOD";
	}

	virtual int16_t get_current_preset_value(const uint8_t index) {
	    int ret =0;
	    // ignoring index - use actual mod dest instead
        uint8_t actual_mod_src = get_current_instasnce();
        if(actual_mod_dest < OSC_PARAM_COUNT*OSC_COUNT) {
            uint8_t actual_osc_instance = actual_mod_dest / OSC_PARAM_COUNT;
            uint8_t actual_osc_knob = actual_mod_dest % OSC_PARAM_COUNT;
            printf("actual_osc_instance %d, actual_osc_knob %d\r\n", actual_osc_instance, actual_osc_knob);
            ret = _osc->get_preset_mod_value(actual_mod_src, actual_osc_instance, actual_osc_knob);
        }
        return ret;
    }

    virtual void set_current_preset_value(const uint8_t index, const uint16_t value) {
        if(!mod_viever_mode) {

        }
        uint8_t actual_mod_src = get_current_instasnce();
        if(actual_mod_dest < OSC_PARAM_COUNT*OSC_COUNT) {
            uint8_t actual_osc_instance = actual_mod_dest / OSC_PARAM_COUNT;
            uint8_t actual_osc_knob = actual_mod_dest % OSC_PARAM_COUNT;
            printf("actual_osc_instance %d, actual_osc_knob %d\r\n", actual_osc_instance, actual_osc_knob);
            _osc->set_preset_mod_value(actual_mod_src, actual_osc_instance, actual_osc_knob, value);
        }
    }

    virtual void select_MOD_dest(const int index) {
        LOG::LOG0("%s dst %d selected\r\n", get_name(), index);
        actual_mod_dest = index;
        reset();
    }

	virtual uint8_t get_midi_ch(){
        return MOD_MIDI_CHANNEL;
    }

	virtual uint8_t get_midi_nr(const uint8_t instance, const uint8_t index) {
        return instance + (index*MOD_COUNT);
    }

	virtual uint8_t get_current_instance_midi_nr(const uint8_t index) {
        LOG::LOG0("%s midi index %d\r\n", get_name(), index);
        return get_midi_nr(get_current_instasnce(), actual_mod_dest);
    }

    virtual void select_function(const uint8_t index) { }
    virtual void force_function(const uint8_t index) { }

private:
	uint8_t actual_mod_dest = 0;

    ADSR *_adsr;
    OSC *_osc;
    LFO *_lfo;
    MOD *_mod;
    FLT *_filter;
    VOL *_vol;
};

#endif /* SRC_MOD_H_ */
