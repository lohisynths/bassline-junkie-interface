/*
 * OSCCTL.h
 *
 *  Created on: Nov 2, 2017
 *      Author: alax
 */

#ifndef SRC_OSCCTL_H_
#define SRC_OSCCTL_H_

#include "utils.h"


#include "Knob.h"
#include "Button.h"
#include "USBMIDI.h"


#define OSC_KNOB_COUNT 5
#define OSC_COUNT 3

#define OSC_BUTTON_COUNT 3

class OSC_CTL {
public:
	OSC_CTL();
	virtual ~OSC_CTL();


	void init(Pwm *leds, int32_t enc_mux_data, int32_t sw_mux_data, USBMIDI *_midi);
	void update(int32_t mux_data, int32_t sw_data);

private:
	Knob knob[OSC_KNOB_COUNT];
	Button sw[OSC_BUTTON_COUNT];

	USBMIDI *midi;




	uint16_t knob_value[OSC_COUNT]={};

	uint8_t current_osc = 0;





	int led_bright = 256;
	int sw_bright = 1024;

	bool osc_loop[OSC_COUNT]={};
	int16_t osc_val[OSC_COUNT][OSC_KNOB_COUNT]={};


};

#endif /* SRC_OSCCTL_H_ */
