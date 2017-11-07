/*
 * OSCCTL.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: alax
 */

#include "OSCCTL.h"

extern bool start;

OSC_CTL::OSC_CTL() : midi(nullptr) {
	// TODO Auto-generated constructor stub

}

OSC_CTL::~OSC_CTL() {
	// TODO Auto-generated destructor stub
}




void OSC_CTL::init(Pwm *leds, int32_t enc_mux_data, int32_t sw_mux_data, USBMIDI *_midi) {
	midi = _midi;


	for (int i = 0; i < OSC_KNOB_COUNT; i++) {
		knob[i].init(i+4, leds, enc_mux_data);
	}

	for (int i = 0; i < OSC_BUTTON_COUNT; i++) {
		sw[i].init(i+4, leds, sw_mux_data);
	}

	sw[current_osc].led_on(sw_bright);

}




void OSC_CTL::update(int32_t mux_data, int32_t sw_data) {

	for (int i = 0; i < OSC_BUTTON_COUNT; i++) {
		bool ret = sw[i].update(sw_data);

		if (ret) {

			bool pushed = !sw[i].get();


#ifdef DEBUG
			printf("OSC ");
			printf("%d", current_osc);
			printf(" switch ");
			printf("%d", i);
			if(pushed)
				printf(" pushed\n");
			else
				printf(" released\n");
#endif
			if(pushed)
			{
				if(i != current_osc)
				{
#ifdef DEBUG
					printf("osc ");
					printf("%d", i);
					printf(" selected\n");
#endif

					sw[i].led_on(sw_bright);

					// get button number of button from current adsr and turn led off
					sw[current_osc].led_on(0);


					current_osc = i;


					for (int i = 0; i < OSC_KNOB_COUNT; i++) {
						knob[i].set_value(osc_val[current_osc][i]);

						int led_nr = knob[i].get_value() / 7;
						knob[i].led_on(led_nr, led_bright);
					}
				}
			}
		}
	}


	for (int i = 0; i < OSC_KNOB_COUNT; i++) {
		knob_msg ret = knob[i].update(mux_data, sw_data);
		if(ret.switch_changed)
		{
			if(i==0 && !knob[i].get_sw_state())
			{
				start^=1;
			}

#ifdef DEBUG
			printf("OSC ");
			printf("%d", current_osc);
			printf(" encoder switch ");
			printf("%d", i);
			if(!knob[i].get_sw_state())
				printf(" pushed\n");
			else
				printf(" released\n");
#endif
		}
		if(ret.value_changed)
		{

			if(i==4)
			{
	            midi->write(
					MIDIMessage::ControlChange(
							(uint8_t)((current_osc*6)+1),
							(uint8_t)(knob[i].get_value()*2))
	            );
			}
			else {
	            midi->write(
					MIDIMessage::ControlChange(
							(uint8_t)((i+(current_osc*6))+3),
							(uint8_t)(knob[i].get_value()*2))
	            );
			}


#ifdef DEBUG
			printf("OSC ");
			printf("%d", current_osc);
			printf(" value ");
			printf("%d", i);
			printf(" changed, ");
			printf("%d\n", knob[i].get_value() );
#endif

			int16_t actual_value = knob[i].get_value();
			osc_val[current_osc][i] = actual_value;

			int led_nr = actual_value / 7;
			knob[i].led_on(led_nr, led_bright);
		}
	}
}
