#include "mbed.h"
#include "Mux.h"
#include "Pwm.h"
#include "Knob.h"

#include "MIDI.h"
#include "USBMIDI.h"
#include "Preset.h"

#include "blocks/OSC.h"
#include "blocks/ADSR.h"
#include "blocks/MOD.h"
#include "blocks/LFO.h"
#include "blocks/FLT.h"
#include "blocks/LED_DISP.h"


MIDI *midi_glob;

void do_message(MIDIMessage msg) {
    switch (msg.type()) {
        case MIDIMessage::NoteOnType:
        	midi_glob->send_note_on(msg.key(), msg.velocity() , 0 );
        	midi_glob->show_message(msg);
            break;
        case MIDIMessage::NoteOffType:
        	midi_glob->send_note_off(msg.key(), msg.velocity() , 0 );
            midi_glob->show_message(msg);
            break;
        default:
            midi_glob->show_message(msg);
    }
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
    bool clear;

    baud(115200);

	USBMIDI midi_usb;
	MIDI midi;
	midi_glob = &midi;

	Pwm leds;
	Mux mux;
	ADSR adsr;
	OSC osc;
	MOD mod;
	LFO lfo;
	FLT filter;
	Preset preset;
	LED_DISP display;

	leds.init();
	mux.init();

	adsr.init(&mux, &leds, &midi);
	osc.init(&mux, &leds, &midi);
	mod.init(&mux, &leds, &midi);
	lfo.init(&mux, &leds, &midi);
	filter.init(&mux, &leds, &midi);
	midi_usb.attach(do_message);
	display.init(&mux, &leds, &midi);

	osc.set_preset(preset.get_osc_preset());
	adsr.set_preset(preset.get_adrsr_preset());
	filter.set_preset(preset.get_flt_preset());
	lfo.set_preset(preset.get_lfo_preset());

	uint16_t tmp1[PWM_DRIVERS_COUNT * PWM_COUNT];
	//bool clear = false;
	auto led_val = leds.get();
	memcpy(tmp1, led_val, sizeof(tmp1));

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}

		//mux.print(1);
		int ret = osc.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+(osc.get_current_osc()*5));
		}

		adsr.update();
		lfo.update();
		ret = filter.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+15);
		}


		ret = display.update();
		if (ret > -1) {
			OSC::preset osc_preset = osc.get_preset();
			ADSR::preset adsr_preset = adsr.get_preset();
			FLT::preset flt_preset = filter.get_preset();

			Preset::SynthPreset piesek;

		    std::copy(osc_preset.begin(), osc_preset.end(), piesek.osc_preset.begin());
		    std::copy(adsr_preset.begin(), adsr_preset.end(), piesek.adsr_preset.begin());
		    std::copy(flt_preset.begin(), flt_preset.end(), piesek.flt_preset.begin());

            preset.save_preset_eeprom(piesek);

			print_preset(SynthPreset);

			if(clear) {
				leds.set(tmp1);
			} else {
				auto led_val = leds.get();
				memcpy(tmp1, led_val, sizeof(tmp1));
				leds.clear();
				leds.update_all();
			}
			clear ^= 1;
		}
		mod.update();
	}
}

