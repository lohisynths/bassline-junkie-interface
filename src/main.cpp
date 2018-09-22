#include "mbed.h"
#include "Mux.h"
#include "Pwm.h"
#include "Knob.h"

#include "MIDI.h"
#include "USBMIDI.h"
#include "EEPROM.h"

#include "blocks/OSC.h"
#include "blocks/ADSR.h"
#include "blocks/MOD.h"
#include "blocks/LFO.h"
#include "blocks/FLT.h"
//#include "blocks/PRESET.h"


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

struct preset {
	OSC::preset osc_preset;
	ADSR::preset adsr_preset;
	FLT::preset flt_preset;
	LFO::preset lfo_preset;
};


/*! \typedef logger
 *  \brief Typedef defining logger used in all instances of this object
 */
typedef logger<MAIN_LOG_LEVEL> LOG;

void print_preset(preset &input){

	uint32_t *data = (uint32_t *)&input;
	for(size_t i=0; i < sizeof(preset)/4; i++) {
		uint32_t ret = data[i];
		LOG::LOG0("preset data nr %zd : %lu\r\n", i, ret);
	}

	for (size_t i = 0; i < OSC_COUNT; i++) {
		for (int j = 0; j < OSC_KNOB_COUNT; j++) {
		    LOG::LOG0("OSC %zd PARAM %d val: %d\r\n", j, i, input.osc_preset[j][i]);
		}
	}
	for (size_t i = 0; i < ADSR_COUNT; i++) {
		for (int j = 0; j < ADSR_PARAM_NR; j++) {
		    LOG::LOG0("ADSR %zd PARAM %d val: %d\r\n", j, i, input.adsr_preset[j][i]);
		}
	}
	for (size_t i = 0; i < FLT_COUNT; i++) {
		for (int j = 0; j < FLT_PARAM_COUNT; j++) {
		    LOG::LOG0("FLT %zd PARAM %d val: %d\r\n", j, i, input.flt_preset[j][i]);
		}
	}
}


void load_preset_eeprom(EEPROM &eeprom, preset &input){
	uint32_t *output_data = (uint32_t *)&input;
	for(size_t i=0; i < sizeof(preset)/4; i++) {
		  uint32_t ret = eeprom.readEEPROMWord(i*4);
		  LOG::LOG0("read nr %zu : %lu\r\n", i, ret);
		  output_data[i] = ret;
	}
}

void save_preset_eeprom(EEPROM &eeprom, preset &input){
	eeprom.erase(sizeof(preset));
	uint32_t *data = (uint32_t *)&input;
	for(size_t i=0; i < sizeof(preset)/4; i++) {
		  uint32_t ret = data[i];
		  eeprom.writeEEPROMWord(i*4, ret);
	}
}

void baud(int baudrate) {
    Serial s(USBTX, USBRX);
    s.baud(baudrate);
}

int main() {
    baud(115200);
    LOG::LOG0("\r\n\r\n----*******--******----\r\n");
    LOG::LOG0("****BASSLINE-JUNKIE****\r\n");
    LOG::LOG0("''**   SIEMANKO    **''\r\n");
    LOG::LOG0("%s at %s\r\n\r\n", __DATE__, __TIME__);

	EEPROM eeprom;
	preset SynthPreset = {0};
//	SynthPreset.osc_preset[0][OSC_PITCH] = 10;
	SynthPreset.osc_preset[0][OSC_SAW] = 127;
	SynthPreset.osc_preset[1][OSC_SAW] = 127;

//	SynthPreset.adsr_preset[0][ADSR_LOOP] = 1;
	SynthPreset.flt_preset[0][FLT_FREQ] = 127;
//	SynthPreset.flt_preset[0][FLT_SHAPE] = 2;
//	SynthPreset.lfo_preset[0][LFO_SHAPE] = 2;
//	SynthPreset.lfo_preset[0][LFO_FREQ] = 64;

//	load_preset_eeprom(eeprom, SynthPreset);
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
	//PRESET display;

	leds.init();
	mux.init();

	adsr.init(&mux, &leds, &midi);
	osc.init(&mux, &leds, &midi);
	mod.init(&mux, &leds, &midi);
	lfo.init(&mux, &leds, &midi);
	filter.init(&mux, &leds, &midi);
	midi_usb.attach(do_message);
	//display.init(&mux, &leds, &midi);

	osc.set_preset(SynthPreset.osc_preset);
	adsr.set_preset(SynthPreset.adsr_preset);
	filter.set_preset(SynthPreset.flt_preset);
	lfo.set_preset(SynthPreset.lfo_preset);

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
			LOG::LOG0("wcisniety %d\r\n", ret);
		}

		adsr.update();
		lfo.update();
		ret = filter.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+15);
			LOG::LOG0("wcisniety %d\r\n", ret);
		}


//		ret = display.update();
//		if (ret > -1) {
//			OSC::osc_preset osc_preset = osc.get_preset();
//			ADSR::adsr_preset adsr_preset = adsr.get_preset();
//			FLT::flt_preset flt_preset = filter.get_preset();
//
//		    std::copy(osc_preset.begin(), osc_preset.end(), SynthPreset.osc_preset.begin());
//		    std::copy(adsr_preset.begin(), adsr_preset.end(), SynthPreset.adsr_preset.begin());
//		    std::copy(flt_preset.begin(), flt_preset.end(), SynthPreset.flt_preset.begin());
//
//			save_preset_eeprom(eeprom, SynthPreset);
//
//			//print_preset(SynthPreset);
//
//			if(clear) {
//				leds.set(tmp1);
//			} else {
//				auto led_val = leds.get();
//				memcpy(tmp1, led_val, sizeof(tmp1));
//				leds.clear();
//				leds.update_all();
//			}
//			clear ^= 1;
//
//		}
//
		mod.update();
	}
}

