#include "mbed.h"
#include "Mux.h"
#include "Pwm.h"
#include "Knob.h"

#include "MIDI.h"
#include "USBMIDI.h"
#include "EEPROM.h"

#include "blocks/ADSR.h"
#include "blocks/OSC.h"
#include "blocks/MOD.h"
#include "blocks/LFO.h"
#include "blocks/FLT.h"
#include "blocks/PRESET.h"


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
	FLT::flt_preset flt_preset;
};

void print_preset(preset &input){

	uint32_t *data = (uint32_t *)&input;
	for(int i=0; i < sizeof(preset)/4; i++) {
		  uint32_t ret = data[i];
		  DEBUG_LOG("preset data. nr %lu : %d\r\n", i, ret);
	}

	for (int i = 0; i < OSC_COUNT; i++) {
		for (int j = 0; j < OSC_KNOB_COUNT; j++) {
			DEBUG_LOG("OSC %d PARAM %d val: %d\r\n", i, j, input.osc_preset[i][j]);
		}
	}
	for (int i = 0; i < ADSR_COUNT; i++) {
		for (int j = 0; j < ADSR_PARAM_NR; j++) {
			DEBUG_LOG("ADSR %d PARAM %d val: %d\r\n", i, j, input.adsr_preset[i][j]);
		}
	}
	for (int i = 0; i < FLT_COUNT; i++) {
		for (int j = 0; j < FLT_PARAM_COUNT; j++) {
			DEBUG_LOG("FLT %d PARAM %d val: %d\r\n", i, j, input.flt_preset[i][j]);
		}
	}
}


void load_preset_eeprom(EEPROM &eeprom, preset &input){
	uint32_t *output_data = (uint32_t *)&input;
	for(int i=0; i < sizeof(preset)/4; i++) {
		  uint32_t ret = eeprom.readEEPROMWord(i*4);
		  DEBUG_LOG("read nr %lu : %d\r\n", i, ret);
		  output_data[i] = ret;
	}
}

void save_preset_eeprom(EEPROM &eeprom, preset &input){
	eeprom.erase(sizeof(preset));
	uint32_t *data = (uint32_t *)&input;
	for(int i=0; i < sizeof(preset)/4; i++) {
		  uint32_t ret = data[i];
		  eeprom.writeEEPROMWord(i*4, ret);
	}
}

int main() {

	EEPROM eeprom;
	preset SynthPreset = {0};
	load_preset_eeprom(eeprom, SynthPreset);
	//print_preset(SynthPreset);

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
	midi_usb.attach(do_message);
	display.init(&mux, &leds, &midi);

	osc.set_preset(SynthPreset.osc_preset);
	adsr.set_preset(SynthPreset.adsr_preset);
	filter.set_preset(SynthPreset.flt_preset);

	uint16_t tmp1[PWM_DRIVERS_COUNT * PWM_COUNT];
	bool clear = false;
	auto led_val = leds.get();
	memcpy(tmp1, led_val, sizeof(tmp1));

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}

		int ret = osc.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+(osc.get_current_osc()*5));
			DEBUG_LOG("wcisniety %d\r\n", ret);
		}

		adsr.update();
		lfo.update();
		ret = filter.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+15);
			DEBUG_LOG("wcisniety %d\r\n", ret);
		}


		ret = display.update();
		if (ret > -1) {
			OSC::osc_preset osc_preset = osc.get_preset();
			ADSR::adsr_preset adsr_preset = adsr.get_preset();
			FLT::flt_preset flt_preset = filter.get_preset();

		    std::copy(osc_preset.begin(), osc_preset.end(), SynthPreset.osc_preset.begin());
		    std::copy(adsr_preset.begin(), adsr_preset.end(), SynthPreset.adsr_preset.begin());
		    std::copy(flt_preset.begin(), flt_preset.end(), SynthPreset.flt_preset.begin());

			uint32_t *presecik = (uint32_t *)&SynthPreset;

			save_preset_eeprom(eeprom, SynthPreset);

			//print_preset(SynthPreset);

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

