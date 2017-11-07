/*
 * ADSRCTL.h
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#ifndef SRC_ADSRCTL_H_
#define SRC_ADSRCTL_H_

#include "utils.h"

#include "Knob.h"
#include "Button.h"
#include "USBMIDI.h"


#define ADSR_KNOB_COUNT 4
#define ADSR_COUNT 3

#define ADSR_BUTTON_COUNT 4


class ADSR_CTL {
public:
	ADSR_CTL();
	virtual ~ADSR_CTL();

	void init(Pwm *leds, int16_t enc_mux_data, int16_t sw_mux_data, USBMIDI *_midi);
	void update(int16_t mux_data, int16_t sw_data);

private:
	Knob knob[ADSR_KNOB_COUNT];
	Button sw[ADSR_BUTTON_COUNT];
	USBMIDI *midi;

	uint16_t knob_value[ADSR_COUNT]={};
	uint8_t current_adsr = 0;

	int led_bright = 256;
	int sw_bright = 1024;

	bool adsr_loop[ADSR_COUNT]={};
	int16_t adsr_val[ADSR_COUNT][ADSR_KNOB_COUNT]={};
};

#endif /* SRC_ADSRCTL_H_ */
