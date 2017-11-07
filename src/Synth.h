/*
 * Synth.h
 *
 *  Created on: Nov 7, 2017
 *      Author: alax
 */

#ifndef SRC_SYNTH_H_
#define SRC_SYNTH_H_

#include "mbed.h"
#include "PCA9685.h"
#include "USBMIDI.h"
#include "Pwm.h"
#include "Mux.h"
#include "ADSRCTL.h"
#include "OSCCTL.h"
#include "ENC8.h"

class Synth {
public:
	Synth();
	virtual ~Synth();

	void run();


private:
	DigitalOut led;
	USBMIDI midi;

	Mux mux_enc;
	Mux mux_sw;
	Pwm leds;


	ADSR_CTL adsr;
	OSC_CTL osc;
	ENC8 enc8;



	bool start = true;


	uint8_t last_note = 0;
	bool last_note_off = true;


};

#endif /* SRC_SYNTH_H_ */
