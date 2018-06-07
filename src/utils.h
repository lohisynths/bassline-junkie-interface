/*
 * utils.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define DEBUG


#define PWM_DRIVERS_COUNT 13
#define PWM_COUNT 16



#define CHECKBIT(n, x)      ((n >> x) & 1)
#define SETBIT(n, x)        (n |=  (1L << x))
#define CLEARBIT(n, x)      (n &= ~(1L << x))
#define TOGGLEBIT(n, x)     (n ^= (1 << x))

#ifdef DEBUG
#define DEBUG_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define  DEBUG_LOG(f_, ...) void( ( (f_), ##__VA_ARGS__ ) )
#endif

#ifdef VERBOSE
#define VERBOSE_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define  VERBOSE_LOG(f_, ...) void( ( (f_), ##__VA_ARGS__ ) )
#endif



#endif /* SRC_UTILS_H_ */
