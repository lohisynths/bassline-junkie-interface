
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
    }
}

int main() {
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

	leds.init();
	mux.init();

	adsr.init(&mux, &leds, &midi);
	osc.init(&mux, &leds, &midi);
	mod.init(&mux, &leds, &midi);
	lfo.init(&mux, &leds, &midi);
	filter.init(&mux, &leds, &midi);
	midi_usb.attach(do_message);         // call back for messages received

	bool rap=false;
	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}

		//		static int j=0;
//		if(!rap) {
//			j++;
//			if(j>10000) j=0;
//		}
//		leds.set(9*16+j, (rap^=1)*1024);
		//mux.print_bit(4, 0);
		//mux.print(3);

		adsr.update();
		osc.update();
		mod.update();
		lfo.update();
		filter.update();
	}
}

