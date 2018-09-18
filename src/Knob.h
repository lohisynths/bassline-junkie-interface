/*
 * Knob.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_KNOB_H_
#define SRC_KNOB_H_

#include "mbed.h"
#include "Pwm.h"
#include "Mux.h"
#include "Enc.h"

class Knob {
public:
	struct knob_msg{
		bool value_changed=false;
		bool switch_changed = false;
	};

	/**
    * A structure to represent knob configuration
    */
	struct knob_init_map {
		////@name Mux related data
		/*@{*/
		Mux *mux;                   /**< pointer to Mux global object */
		uint16_t *mux_raw_data;     /**< pointer to raw mux data */
		uint8_t mux_first_bit;      /**< first bit in mux_raw_data  */
		uint16_t encoder_max_value; /**< the x coordinate */
		/*@}*/
		////@name LED related data
		/*@{*/
		Pwm *leds;                  /**< pointer to Pwm global object */
		uint8_t max_led_value;      /**< leds maximum value <br><br>
	                                     maximum value = 12 bit = 4096 */
		uint8_t first_pwm_output;   /**< first led output from pwm driver led array
		                                 maximum value = PWM_DRIVERS_COUNT * 16 */
		uint8_t total_led_count;    /**< total led count around encoder */
		/*@}*/
	};

	Knob();
	virtual ~Knob();

	void init(knob_init_map config);
	knob_msg update();
	void set_leds(uint16_t value);
	void set_value(uint16_t val);
	uint16_t get_knob_value();
	bool get_switch_state();
	knob_init_map knob_config={};

private:
	void led_on_last_off(size_t led_nr, int16_t bright);
	void print_config(knob_init_map config);

	Enc encoder;

	uint16_t encoder_value = 0;
	int16_t encoder_value_last = 0;
	uint16_t led_last = 0;
	uint16_t last_sw_state = 1;

	float encoder_max_to_127_divider;
	float encoder_2_midi_mult;
};

#endif /* SRC_KNOB_H_ */
