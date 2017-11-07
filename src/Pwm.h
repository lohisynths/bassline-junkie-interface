/*
 * Pwm.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include "../PCA9685.h"
#include "utils.h"

class Pwm {
public:
	Pwm();
	virtual ~Pwm();

	void set(size_t led, uint16_t val);
	void init();
	void update_all();
	void clear();

	PCA9685	 pwm[PWM_DRIVERS_COUNT];

	uint8_t twbrbackup=0;


};

#endif /* SRC_PWM_H_ */
