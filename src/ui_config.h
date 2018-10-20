/*
 * ui_config.h
 *
 *  Created on: Sep 26, 2018
 *      Author: alax
 */

#ifndef SRC_UI_CONFIG_H_
#define SRC_UI_CONFIG_H_

#include "blocks/ADSR.h"
#include "blocks/OSC.h"
#include "blocks/FLT.h"
#include "blocks/MOD.h"


ADSR::knob_config adsr_knob_config = {
    Knob::knob_init_map{0, 9, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (ADSR_FIRST_ENC_LED + 30), COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{0, 6, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (ADSR_FIRST_ENC_LED + 20), COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{0, 3, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (ADSR_FIRST_ENC_LED + 10), COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{0, 0, KNOB_MAX_VAL,  KNOB_MAX_LED_VAL, (ADSR_FIRST_ENC_LED + 0),  COMMON_KNOB_LED_COUNT},
};

ADSR::button_config adsr_button_config = {
    Button::button_init_map{0, 15, KNOB_MAX_LED_VAL, (ADSR_FIRST_BUTTON_LED+3)},
    Button::button_init_map{0, 14, KNOB_MAX_LED_VAL, (ADSR_FIRST_BUTTON_LED+2)},
    Button::button_init_map{0, 13, KNOB_MAX_LED_VAL, (ADSR_FIRST_BUTTON_LED+1)},
    Button::button_init_map{0, 12, KNOB_MAX_LED_VAL, (ADSR_FIRST_BUTTON_LED+0)}
};

OSC::knob_config osc_knob_ctrl={
    Knob::knob_init_map{1, 12,KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (OSC_FIRST_ENC_LED + 48),COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{1, 9, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (OSC_FIRST_ENC_LED + 30),COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{1, 6, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (OSC_FIRST_ENC_LED + 20),COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{1, 3, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (OSC_FIRST_ENC_LED + 10),COMMON_KNOB_LED_COUNT},
    Knob::knob_init_map{1, 0, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (OSC_FIRST_ENC_LED + 0) ,COMMON_KNOB_LED_COUNT}
};

OSC::button_config osc_button_ctrl={
    Button::button_init_map{3, 3, KNOB_MAX_LED_VAL, (OSC_FIRST_BUTTON_LED+2)},
    Button::button_init_map{3, 2, KNOB_MAX_LED_VAL, (OSC_FIRST_BUTTON_LED+1)},
    Button::button_init_map{1, 15, KNOB_MAX_LED_VAL, (OSC_FIRST_BUTTON_LED)}};


FLT::knob_config flt_knob_ctrl={
    Knob::knob_init_map{3, 4, FLT_BIG_KNOB_MAX_VAL, COMMON_KNOB_LED_COUNT, (FLT_FIRST_ENC_LED + 0), FLT_BIG_KNOB_LED_COUNT},
    Knob::knob_init_map{3, 7, KNOB_MAX_VAL , COMMON_KNOB_LED_COUNT, (FLT_FIRST_ENC_LED + 16), COMMON_KNOB_LED_COUNT}
};

FLT::button_config flt_button_ctrl={
    Button::button_init_map{3, 15, BUTTON_MAX_LED_VAL, (FLT_FIRST_BUTTON_LED+2)},
    Button::button_init_map{3, 14, BUTTON_MAX_LED_VAL, (FLT_FIRST_BUTTON_LED+1)},
    Button::button_init_map{3, 13, BUTTON_MAX_LED_VAL, (FLT_FIRST_BUTTON_LED+0)}
};

MOD::knob_config mod_knob_ctrl={
    2, 0, KNOB_MAX_VAL,  KNOB_MAX_LED_VAL, (MOD_FIRST_ENC_LED +  0), COMMON_KNOB_LED_COUNT
};

MOD::button_config mod_button_ctrl={
    2, 8, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 5),
    2, 7, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 4),
    2, 6, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 3),
    2, 5, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 2),
    2, 4, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 1),
    2, 3, BUTTON_MAX_LED_VAL, (MOD_FIRST_BUTTON_LED + 0)
};


LFO::knob_config lfo_knob_ctrl={
    Knob::knob_init_map{4, 0, KNOB_MAX_VAL, KNOB_MAX_LED_VAL, (LFO_FIRST_ENC_LED +  0), COMMON_KNOB_LED_COUNT},
};

LFO::button_config lfo_button_ctrl={
    Button::button_init_map{3, 0,  BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 20)},
    Button::button_init_map{2, 15, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 19)},
    Button::button_init_map{2, 14, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 18)},

    Button::button_init_map{2, 13, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 4)},
    Button::button_init_map{2, 12, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 3)},
    Button::button_init_map{2, 11, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 2)},
    Button::button_init_map{2, 10, BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 1)},
    Button::button_init_map{2, 9,  BUTTON_MAX_LED_VAL, (LFO_FIRST_BUTTON_LED + 0)}
};

LED_DISP::knob_config disp_knob_ctrl = {
    Knob::knob_init_map{4, 6, DISPLAY_KNOB_MAX_VAL, DISPLAY_MAX_VAL, 0, 0 }
};

LED_DISP::button_config disp_button_ctrl;


VOL::knob_config vol_knob_ctrl = {
    Knob::knob_init_map{4, 3, KNOB_MAX_VAL, 0, 0, 0 }
};

VOL::button_config vol_button_ctrl;

#endif /* SRC_UI_CONFIG_H_ */
