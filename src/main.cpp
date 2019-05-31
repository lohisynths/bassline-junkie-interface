#include "mbed.h"
#include "Mux.h"
#include "Pwm.h"
#include "Knob.h"

#include "MIDI.h"
#include "USBMIDI.h"
#include "Preset.h"

#include "blocks/OSC.h"
#include "blocks/ADSR.h"
#include "blocks/LFO.h"
#include "blocks/FLT.h"
#include "blocks/LED_DISP.h"
#include "blocks/VOL.h"
#include "blocks/MOD.h"

#include "ui_config.h"

#include "platform/CircularBuffer.h"

#define BUF_SIZE 128
CircularBuffer<MIDIMessage, BUF_SIZE> buf;


void do_message(MIDIMessage msg) {
	buf.push(msg);
}

/*! \typedef logger
 *  \brief Typedef defining logger used in all instances of this object
 */
typedef logger<MAIN_LOG_LEVEL> LOG;


void baud(int baudrate) {
    Serial s(USBTX, USBRX);
    s.baud(baudrate);

    LOG::LOG0("\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    LOG::LOG0("\t\t\t     ----******---******----\r\n");
    LOG::LOG0("\t\t\t     ****BASSLINE--JUNKIE***\r\n");
    LOG::LOG0("\t\t\t     ''**    SIEMANKO   **''\r\n");
    LOG::LOG0("\t\t\t     %s at %s\r\n", __DATE__, __TIME__);
    LOG::LOG0("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
}

int main() {



	Pwm leds;
	Mux mux;



    baud(115200);

	leds.init();
	mux.init();

	// set following const for debug prints
	//const LOG_LEVELS MUX_LOG_LEVEL = set_level(LOG_LEVELS::DISABLED);
	mux.update();
	int i =0;
	while(1) {
		if(i > (1<<11)) {
			i = 0;
		}
		leds.set_all(i++);
		//wait(0.01);
	}
}

