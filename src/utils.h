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

#define KNOB_MAX_VAL                (10)
#define KNOB_MAX_LED_VAL 			(50)
#define COMMON_KNOB_LED_COUNT 		(10)
#define BUTTON_MAX_LED_VAL 			(50)

#define CHECKBIT(n, x)      ((n >> x) & 1)
#define SETBIT(n, x)        (n |=  (1L << x))
#define CLEARBIT(n, x)      (n &= ~(1L << x))
#define TOGGLEBIT(n, x)     (n ^= (1 << x))

#endif /* SRC_UTILS_H_ */
