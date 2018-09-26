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




#endif /* SRC_UI_CONFIG_H_ */
