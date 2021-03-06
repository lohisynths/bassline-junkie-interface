/*
 * ADSRCTL.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ADSRCTL_H_
#define SRC_ADSRCTL_H_

#include "utils.h"

#include "Knob.h"
#include "Button.h"

#include <array>

#define MOD_SRC_COUNT 6

template<uint8_t KNOB_COUNT, uint8_t BUTTON_COUNT, uint8_t PARAM_COUNT, uint8_t COUNT>
class UI_BLOCK {
public:
	typedef std::array<std::array<int, PARAM_COUNT>, COUNT> preset;
    typedef std::array<std::array<int, PARAM_COUNT*MOD_SRC_COUNT>, MOD_SRC_COUNT> mod_preset;
	typedef std::array<Knob::knob_init_map, KNOB_COUNT> knob_config;
	typedef std::array<Button::button_init_map, BUTTON_COUNT> button_config;

	// masakra ^-^
	struct ret_value {
	    bool buttons_changed = false;
	    std::array<bool, BUTTON_COUNT> buttons = {};
        bool knobs_sw_changed = false;
        std::array<bool, KNOB_COUNT> knobs_sw;
        bool knobs_changed = false;
        std::array<bool, KNOB_COUNT> knobs_sw_changed_array = {};
        std::array<int, KNOB_COUNT> knobs_val;
        int get_first_pushed_knob_sw() {
            int index = -1;
            for(unsigned int i = 0; i < KNOB_COUNT; i++) {
                if(knobs_sw_changed_array[i]) {
                    if(knobs_sw[i]) {
                        return i;
                    }
                }
            }
            return index;
        }
	};

    mod_preset mod_preset_values = {};
    bool mod_viever_mode = false;


    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<UI_BLOCK_LOG_LEVEL> LOG;

	UI_BLOCK(){};
	virtual ~UI_BLOCK(){};


    void init(knob_config knob_ctrl, button_config button_ctrl,
            MIDI *_midi, Pwm *_leds, Mux *_mux) {
        midi = _midi;
        leds = _leds;
        mux = _mux;
        init_internal(knob_ctrl, button_ctrl);
        select_instance(current_instance);
    }

    uint8_t get_current_instasnce() {
        return current_instance;
    }

    void turn_off_sw(uint8_t index) {
        sw[index].set_led_val(0);
    }
    void turn_on_sw(uint8_t index) {
        sw[index].set_led_val(BUTTON_MAX_LED_VAL);
    }

    virtual int16_t get_current_preset_value(uint8_t index) {
        return get_preset_value(current_instance, index);
    }

    virtual int16_t get_preset_mod_value(uint8_t src, uint8_t dst) {
          return mod_preset_values[src][dst];
    }

    virtual void set_preset_mod_value(uint8_t src, uint8_t dst, int value) {
          mod_preset_values[src][dst] = value;
    }

    virtual int16_t get_preset_value(uint8_t instance, uint8_t index) {
        if (!mod_viever_mode) {
            return preset_values[instance][index];
        } else {
            return get_preset_mod_value(instance, index);
        }
    }

    virtual void set_preset_value(uint8_t instance, uint8_t index, uint8_t value) {
        if(!mod_viever_mode) {
            preset_values[instance][index] = value;
        } else {
            set_preset_mod_value(instance, index, value);
        }
    }

    virtual void set_current_preset_value(uint8_t index, uint16_t value) {
        set_preset_value(current_instance, index, value);
    }

    std::array<Button, BUTTON_COUNT> &get_sw(){
        return sw;
    };

    std::array<Knob, KNOB_COUNT> &get_knobs(){
        return knob;
    };

    Pwm* get_leds(){
        return leds;
    }

    MIDI* get_midi(){
        return midi;
    }

    void set_preset(preset input) {
        memcpy(&preset_values, &input, sizeof(input));
        select_instance(current_instance);
        dump_midi();
    };

    void set_mod_preset(mod_preset input) {
        memcpy(&mod_preset_values, &input, sizeof(input));
        select_instance(current_instance);
        dump_midi();
    };

    preset &get_preset() {
        return preset_values;
    }

    mod_preset &get_mod_preset() {
        return mod_preset_values;
    }

    ret_value update() {
        memset(&last_ret, 0, sizeof(last_ret));
        update_buttons(last_ret);
        update_knobs(last_ret);
        return last_ret;
    }

    int get_first_knob_sw_pushed() {
        int ret = -1;
        if (last_ret.knobs_sw_changed) {
            ret =  last_ret.get_first_pushed_knob_sw();
        }
        return ret;
    }

    bool get_knob_changed() {
        return last_ret.knobs_changed;
    }

    void set_viewer_mode(bool enable) {
        mod_viever_mode = enable;
        select_instance(get_current_instasnce());
    }

    //needed only by mod class - do something with it
    void reset() {
        select_instance(current_instance);
    }


    virtual uint8_t get_current_instance_midi_nr(uint8_t index) {
        return get_midi_nr(get_current_instasnce(), index);
    }

private:
	void init_internal(knob_config &knob_settings, button_config &button_config) {
		for (int i = 0; i < KNOB_COUNT; i++) {
			knob[i].init(knob_settings[i], leds, mux);
		}
		for (int i = 0; i < BUTTON_COUNT; i++) {
			sw[i].init(button_config[i], leds, mux);
		}
	}

	void update_knobs(ret_value &input) {
        for (uint8_t i = 0; i < KNOB_COUNT; i++) {
            Knob::knob_msg ret = knob[i].update();
            if (ret.switch_changed) {
                bool state = !knob[i].get_switch_state();
                LOG::LOG0("%s %d encoder switch %d ", get_name(), current_instance, i);
                LOG::LOG0( (!state) ? "pushed\r\n" : "released\r\n" );
                knob_sw_changed(i, state);
                input.knobs_sw[i] = state;
                input.knobs_sw_changed = true;
                input.knobs_sw_changed_array[i] = true;
            }
            if (ret.value_changed) {
                LOG::LOG0("%s %d knob %d changed %d\r\n", get_name(), current_instance, i, knob[i].get_knob_value());
                knob_val_changed(i, knob[i].get_knob_value());
                input.knobs_val[i] = knob[i].get_knob_value();
                input.knobs_changed = true;
            }
        }
    }

	void button_changed(uint8_t index, bool state) {
		if (state) {
			// if COUNT = 1 only one knob used, one instance,
			// jump straight to special function
			// if i < COUNT then select instance
			// common behavior for every class with buttons
			if(COUNT != 1) {
				if (index < COUNT) {
					select_instance(index);
				} else {
					select_function(index-COUNT);
				}
			} else {
				select_function(index);
			}
		}
	};

	virtual void knob_val_changed(uint8_t index, uint16_t value_scaled) {
        set_current_preset_value(index, value_scaled);
		knob[index].led_indicator_set_value(value_scaled);
		midi->send_cc(get_current_instance_midi_nr(index), value_scaled, get_midi_ch());
	}

	void force_knob_update(uint8_t index, uint16_t value_scaled) {
        set_current_preset_value(index, value_scaled);
		knob[index].set_value(value_scaled);
        knob[index].led_indicator_set_value(value_scaled, true);
	}

	void update_buttons(ret_value &input) {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			bool ret = sw[i].update();
			if (ret) {
				bool pushed = !sw[i].get_state();
				LOG::LOG0("%s %d button switch %d", get_name(), current_instance, i);
				LOG::LOG0((pushed) ? "pushed\r\n" : "released\r\n" );
				button_changed(i, pushed);
				input.buttons[i] = pushed;
                input.buttons_changed = true;
			}
		}
	};

    void select_instance(uint8_t index) {
        // force update for select instance
        // if index == current instance
        // turn off index led and turn on
        // get button number of button from current OSC and turn led off
        // templetize this shit
        if(BUTTON_COUNT > 0) {
            turn_off_sw(current_instance);
            turn_on_sw(index);
        }
        current_instance = index;

        for (int j = 0; j < KNOB_COUNT; j++) {
            // get value of [i] knob from current preset
            //
            uint8_t val = get_current_preset_value(j);
            force_knob_update(j, val);
        }

        if(!mod_viever_mode) {
            const int special_parameters_count = PARAM_COUNT - KNOB_COUNT;
            for (int i = 0; i < special_parameters_count; i++) {
                uint16_t val = get_current_preset_value(KNOB_COUNT+i);
                force_function(val);
                LOG::LOG0("%s %d UI_BLOCK special param %d %d \r\n", get_name(), current_instance, KNOB_COUNT+i, val);
            }
        }

        LOG::LOG0("%s %d selected\r\n", get_name(), index);
    };

	void dump_midi() {
        LOG::LOG0("%s %d UI_BLOCK dump midi\r\n", get_name());
        for (int i = 0; i < COUNT; i++) {
            for (int j = 0; j < PARAM_COUNT; j++) {
                int value = preset_values[i][j];
                uint8_t midi_nr = get_midi_nr(i, j);
                midi->send_cc(midi_nr, value, get_midi_ch());
            }
        }
	}

	virtual void knob_sw_changed(uint8_t index, bool state) {};
	virtual void select_function(uint8_t index) = 0;
	virtual void force_function(uint8_t index) = 0;
    virtual const char* get_name() = 0;

    virtual uint8_t get_midi_nr(uint8_t instance, uint8_t index) = 0;
    virtual uint8_t get_midi_ch() = 0;

    preset preset_values = {};


	MIDI *midi;
	Pwm *leds;
	Mux *mux;
    ret_value last_ret;

    std::array<Button, BUTTON_COUNT> sw;
    std::array<Knob, KNOB_COUNT> knob;

    uint8_t current_instance = 0;
};

#endif /* SRC_ADSRCTL_H_ */
