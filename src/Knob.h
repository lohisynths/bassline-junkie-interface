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


struct knob_msg{
	bool value_changed=false;
	bool switch_changed = false;
};










class Knob {
public:

	struct knob_map {
		Mux *mux;
		uint8_t mux_nr;
		uint8_t enc_bit;
		uint16_t knob_max_val;

		Pwm *leds;
		uint8_t first_led;
		uint8_t led_count;
		uint8_t led_max_val;
	};


	virtual ~Knob();
	knob_msg update();

	void init(uint8_t led_index, uint8_t mux_index, Pwm &pwm, uint16_t &mux_data, uint16_t max_val, uint8_t leds_count);

	void init(knob_map config);


	void reset(int16_t mux_data);


	void led_on(size_t led_nr, int16_t bright);

	void set_leds(uint16_t value);

	int16_t get_value();
	int16_t get_value_scaled();

	void set_value(uint16_t val);

	bool get_sw_state();
	uint16_t get_max(){return knob_max_val;};


private:

	Pwm *leds;
	uint16_t *m_mux_data;


	Enc encoder;


	uint16_t encoder_value = 0;
	int16_t enc_last = 0;
	uint16_t led_last = 0;
	uint16_t last_sw = 1;

	uint16_t knob_max_val = 0;
	uint16_t knob_leds_count = 0;

	float divider;
	float val_divider;
	/////////////////////
	// mux and pwm registers bit numbers
	uint8_t m_first_led = 0;
	uint8_t m_mux_index = 0;




};

#endif /* SRC_KNOB_H_ */
