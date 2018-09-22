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

#define KNOB_MAX_LED_VAL 			(255)
#define COMMON_KNOB_LED_COUNT 		(10)

#define BUTTON_MAX_LED_VAL 			(511)

#define CHECKBIT(n, x)      ((n >> x) & 1)
#define SETBIT(n, x)        (n |=  (1L << x))
#define CLEARBIT(n, x)      (n &= ~(1L << x))
#define TOGGLEBIT(n, x)     (n ^= (1 << x))

#ifdef DEBUG
#define DEBUG_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define  DEBUG_LOG(f_, ...)  (void)0
#endif

#ifdef DEBUG_PCA9685
#define DEBUG_PCA9685_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define  DEBUG_PCA9685_LOG(f_, ...)  (void)0
#endif

#ifdef DEBUG_KNOB
#define DEBUG_KNOB_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define  DEBUG_KNOB_LOG(f_, ...)  (void)0
#endif

#endif /* SRC_UTILS_H_ */
