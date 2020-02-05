/*
 * Pwm.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include "PCA9685.h"
#include "utils.h"

class Pwm {
public:
	Pwm();
	virtual ~Pwm();

	void set(uint8_t led, uint16_t val);
	void set(const uint16_t input[PWM_DRIVERS_COUNT * PWM_COUNT]);
	const uint16_t *get();

	void init();
	void update_all();
	void clear();

	PCA9685	 pwm[PWM_DRIVERS_COUNT];

    void backup_state();
    void restore_state();

    uint16_t tmp1[PWM_DRIVERS_COUNT * PWM_COUNT];
    uint16_t pwm_array[PWM_DRIVERS_COUNT * PWM_COUNT];


};

#endif /* SRC_PWM_H_ */
