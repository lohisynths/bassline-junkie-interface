/*
 * utils.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define DEBUG


#define PWM_DRIVERS_COUNT 7
#define PWM_COUNT 16

#define KNOB_COUNT 9
#define BUTTON_COUNT 7

#define ENC_MUX_INPUT_PIN 4
#define SW_MUX_INPUT_PIN 3

#define SW_ENC8_INPUT_PIN 5
#define ENC_ENC8A_INPUT_PIN 6
#define ENC_ENC8B_INPUT_PIN 7

#define CHECKBIT(n, x)      ((n >> x) & 1)
#define SETBIT(n, x)        (n |=  (1L << x))
#define CLEARBIT(n, x)      (n &= ~(1L << x))
#define TOGGLEBIT(n, x)     (n ^= (1 << x))



#endif /* SRC_UTILS_H_ */
