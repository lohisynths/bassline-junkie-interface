/*
 * ADSRCTL.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "ADSRCTL.h"

ADSR_CTL::ADSR_CTL() {

}

ADSR_CTL::~ADSR_CTL() {

}


enum ADSR_BUTTONS{
	ADSR0,
	ADSR1,
	ADSR2,
	LOOP
};

void ADSR_CTL::init(Pwm &leds, uint16_t &enc_mux_data) {
	m_mux_data = &enc_mux_data;
	m_leds = &leds;

	for (int i = 0; i < ADSR_KNOB_COUNT; i++) {
		knob[i].init(i*10, i*3, *m_leds, *m_mux_data);
	}

	for (int i = 0; i < ADSR_BUTTON_COUNT; i++) {
		sw[i].init(40+i, 12+i, *m_leds, *m_mux_data);
	}

	sw[current_adsr].led_on(sw_bright);

}


void ADSR_CTL::update_buttons() {
	for (int i = 0; i < ADSR_BUTTON_COUNT; i++) {
		bool ret = sw[i].update();

		if (ret) {

			bool pushed = !sw[i].get();
#ifdef DEBUG
			printf("ADSR ");
			printf("%d ", current_adsr);
			printf(" switch ");
			printf("%d ", i);
			if(pushed)
				printf(" pushed\r\n");
			else
				printf(" released\r\n");
#endif

			if(pushed)
			{
				if(i == LOOP)
				{
					adsr_loop[current_adsr] ^= 1;

					sw[i].led_on(adsr_loop[current_adsr] * sw_bright);

#ifdef DEBUG
					printf("current_adsr ");
					printf("%d", current_adsr);
					if(adsr_loop[current_adsr]) {
						printf(" LOOP ON\r\n");
					}
					else {
						printf(" LOOP OFF\r\n");
					}
#endif

				}
				else
				{

					if(i != current_adsr)
					{

#ifdef DEBUG
						printf("adsr ");
						printf("%d ",i);
						printf(" selected\r\n");
#endif
						sw[i].led_on(sw_bright);

						// get button number of button from current adsr and turn led off
						sw[current_adsr].led_on(0);


						current_adsr = i;


						for (int i = 0; i < ADSR_KNOB_COUNT; i++) {
							knob[i].set_value(adsr_val[current_adsr][i]);

							int led_nr = knob[i].get_value() / 7;
							knob[i].led_on(led_nr, led_bright);
						}

						sw[LOOP].led_on(adsr_loop[current_adsr] * sw_bright);
					}
				}
			}
		}
	}
}


void ADSR_CTL::update() {

	update_buttons();

	for (uint8_t i = 0; i < ADSR_KNOB_COUNT; i++) {
		knob_msg ret = knob[i].update();
		if(ret.switch_changed)
		{
#ifdef DEBUG
			printf("ADSR ");
			printf("%d\n", current_adsr);
			printf(" encoder switch ");
			printf("%d\n", i);
			if(!knob[i].get_sw_state())
				printf(" pushed\n");
			else
				printf(" released\n");
#endif
		}
		if(ret.value_changed)
		{
//            midi->write(
//            		MIDIMessage::ControlChange(
//            				(uint8_t)((i+(current_adsr*ADSR_KNOB_COUNT))+20),
//							(uint8_t)(knob[i].get_value()*2))
//            );


#ifdef DEBUG
			printf("ADSR ");
			printf("%d",current_adsr);
			printf(" value ");
			printf("%d",i);
			printf(" changed, ");
			printf("%d\r\n", knob[i].get_value() );
			////// DEBUG
#endif
			int16_t actual_value = knob[i].get_value();
			adsr_val[current_adsr][i] = actual_value;

			int led_nr = actual_value / 7;
			knob[i].led_on(led_nr, led_bright);
		}
	}
}
