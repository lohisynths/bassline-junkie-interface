/*
 * utils.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include "logger.h"

#define PWM_DRIVERS_COUNT 13
#define PWM_COUNT 16

#define KNOB_MAX_VAL                (32)
#define KNOB_MAX_LED_VAL 			(50)
#define COMMON_KNOB_LED_COUNT 		(10)
#define BUTTON_MAX_LED_VAL 			(256)
#define DISPLAY_MAX_LED_VAL         (256)

#define CHECKBIT(n, x)      ((n >> x) & 1)
#define SETBIT(n, x)        (n |=  (1L << x))
#define CLEARBIT(n, x)      (n &= ~(1L << x))
#define TOGGLEBIT(n, x)     (n ^= (1 << x))

const LOG_LEVELS MIN = static_cast<LOG_LEVELS>(0);
const LOG_LEVELS MAX = static_cast<LOG_LEVELS>(0xFF);

const LOG_LEVELS KNOB_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS BUTTON_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);

const LOG_LEVELS PRESET_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);

const LOG_LEVELS PCA9685_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS MUX_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);

const LOG_LEVELS UI_BLOCK_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS MAIN_LOG_LEVEL = MAX;

const LOG_LEVELS ADSR_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS OSC_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS LFO_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS FLT_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS MOD_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
const LOG_LEVELS MIDI_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);

#endif /* SRC_UTILS_H_ */
