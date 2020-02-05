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

/*! \var bool shit_init
 *  \brier The very important variable which is responsible for some shity initialization on synthesizer preset.
 *
 *   All of the presets will be erased if shit_init = true.
 *   Use if  preset.update_preset() after  preset.load_global() causes problems.
 */
const bool shit_init = false;


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
//	USBMIDI midi_usb;
	MIDI midi;
	Pwm leds;
	Mux mux;
	Preset preset;

	ADSR adsr;
	OSC osc;
    LFO lfo;
	MOD mod;
	FLT filter;
	LED_DISP display;
    VOL vol;

    baud(115200);
//	midi_usb.attach(do_message);
	leds.init();
	mux.init();
	if(shit_init) {
	  preset.erase();
	} else {
	  preset.load_global(0);
	}

    mod.init_(&adsr, &osc, &lfo, &mod, &filter, &vol);

	adsr.init(adsr_knob_config, adsr_button_config, &midi, &leds, &mux);
	osc.init(osc_knob_ctrl, osc_button_ctrl, &midi, &leds, &mux);
    filter.init(flt_knob_ctrl, flt_button_ctrl, &midi, &leds, &mux);
	mod.init(mod_knob_ctrl, mod_button_ctrl, &midi, &leds, &mux);
	lfo.init(lfo_knob_ctrl, lfo_button_ctrl, &midi, &leds, &mux);
	display.init(disp_knob_ctrl, disp_button_ctrl, &midi, &leds, &mux);
	vol.init(vol_knob_ctrl, vol_button_ctrl, &midi, &leds, &mux);

    preset.init(&adsr, &osc, &lfo, &mod, &filter, &vol, &display);
    preset.update_preset();

	while(1) {

		while(buf.size() > 0) {
			MIDIMessage msg = {};
			buf.pop(msg);
		    switch (msg.type()) {
		        case MIDIMessage::NoteOnType:
		        	midi.send_note_on(msg.key(), msg.velocity() , 0 );
		        	midi.show_message(msg);
		            break;
		        case MIDIMessage::NoteOffType:
		        	midi.send_note_off(msg.key(), msg.velocity() , 0 );
		        	midi.show_message(msg);
		            break;
		        default:
		        	midi.show_message(msg);
		    }
		}
		mux.update();
		vol.update();
		osc.update();
        adsr.update();
        lfo.update();
        filter.update();
        display.update();
        mod.update();

        mod.update2();
        preset.update();
	}
}

