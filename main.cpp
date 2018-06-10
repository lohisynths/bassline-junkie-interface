
#include "mbed.h"
#include "src/Mux.h"
#include "src/Pwm.h"
#include "src/Knob.h"

#include "src/MIDI.h"

#include "src/blocks/ADSR.h"
#include "src/blocks/OSC.h"
#include "src/blocks/MOD.h"
#include "src/blocks/LFO.h"
#include "src/blocks/FLT.h"
#include "src/blocks/PRESET.h"

#include <algorithm>

#include "USBMIDI.h"
MIDI *midi_glob;

void do_message(MIDIMessage msg) {
    switch (msg.type()) {
        case MIDIMessage::NoteOnType:
        	midi_glob->send_note_on(msg.key(), msg.velocity() , 0 );
            break;
        case MIDIMessage::NoteOffType:
        	midi_glob->send_note_off(msg.key(), msg.velocity() , 0 );
            break;
        default:
			printf("midi err \r\n");
    }
}

struct preset {
	OSC::osc_preset osc_preset;
	ADSR::adsr_preset adsr_preset;
};

preset SynthPreset = {
	.osc_preset = {{
		{63, 0, 0, 0, 63},
		{63, 0, 63, 0, 63},
		{0, 63, 0, 0, 63}
	}},
	.adsr_preset = {{
		{0, 32, 64, 0, 1},
		{0, 32, 64, 0, 1},
		{0, 32, 64, 0, 1}
	}}
};

#include "src/EEPROM.h"

int main() {

	auto lolo = sizeof(SynthPreset);

	EEPROM eeprom;
	eeprom.erase();
	eeprom.write();
	eeprom.read();

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
	PRESET display;

	leds.init();
	mux.init();

	adsr.init(&mux, &leds, &midi);
	osc.init(&mux, &leds, &midi);
	mod.init(&mux, &leds, &midi);
	lfo.init(&mux, &leds, &midi);
	filter.init(&mux, &leds, &midi);
	midi_usb.attach(do_message);         // call back for messages received
	display.init(&mux, &leds, &midi);

	osc.set_preset(SynthPreset.osc_preset);
	adsr.set_preset(SynthPreset.adsr_preset);

	bool rap=false;
	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}

		static int j=6;
		if(!rap) {
			j++;
			if(j>32) j=0;
		}
		//wait(0.1);
		//leds.set(11*16+j, (rap^=1)*1024);


		//mux.print_bit(4, 7);
		//mux.print_bit(4, 8);

		//mux.print(4);

		int ret = osc.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+(osc.get_current_osc()*5));
			printf("wcisniety %d\r\n", ret);
		}

		adsr.update();
		lfo.update();
		ret = filter.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+15);
			printf("wcisniety %d\r\n", ret);
		}

		ret = display.update();
		if (ret > -1) {
			OSC::osc_preset osc_preset = osc.get_preset();
			ADSR::adsr_preset adsr_preset = adsr.get_preset();

		    std::copy(osc_preset.begin(), osc_preset.end(), SynthPreset.osc_preset.begin());
		    std::copy(adsr_preset.begin(), adsr_preset.end(), SynthPreset.adsr_preset.begin());

			for (int i = 0; i < OSC_COUNT; i++) {
				for (int j = 0; j < OSC_KNOB_COUNT; j++) {
					printf("OSC %d PARAM %d val: %d\r\n", i, j, SynthPreset.osc_preset[i][j]);
				}
			}
			for (int i = 0; i < ADSR_COUNT; i++) {
				for (int j = 0; j < ADSR_PARAM_NR; j++) {
					printf("ADSR %d PARAM %d val: %d\r\n", i, j, SynthPreset.adsr_preset[i][j]);
				}
			}
		}


		mod.update();
	}
}

