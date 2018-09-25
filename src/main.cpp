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

	preset.load_global(0);

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
    mod.set_preset(preset.get_mod_preset());

    bool last_disp_pushed = false;
    int last_disp_count = 0;

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}

		//mux.print(1);
		int ret = osc.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+(osc.get_current_osc()*5));
            mod.update_instance();
		}

		adsr.update();
		lfo.update();
		ret = filter.update();
		if (ret > -1) {
			mod.select_MOD_dest(ret+15);
			mod.update_instance();
		}

        ret = display.update();


		bool pushed = display.if_knob_sw_pushed();
		if(last_disp_pushed != pushed) {
		    LOG::LOG0("changed %d\r\n", pushed);
		}

		if(pushed) {
		    last_disp_count++;
		}

		if(!pushed && last_disp_pushed) {
		    if(last_disp_count > 500) {
	            LOG::LOG0("saving preset %d\r\n", display.get_actual_preset_nr());
//	            leds.backup_state();
	            Preset::SynthPreset asdsa = {
	                    osc.get_preset(),
	                    adsr.get_preset(),
	                    filter.get_preset(),
	                    lfo.get_preset(),
	                    mod.get_preset()
	            };
	            preset.save_preset_eeprom(asdsa, display.get_actual_preset_nr());
//	            leds.restore_state();
                LOG::LOG0("preset %d saved\r\n", display.get_actual_preset_nr());

		    } else {
                LOG::LOG0("load preset %d\r\n", display.get_actual_preset_nr());
//                leds.backup_state();
                preset.load_preset_eeprom(display.get_actual_preset_nr());

                osc.set_preset(preset.get_osc_preset());
                adsr.set_preset(preset.get_adrsr_preset());
                filter.set_preset(preset.get_flt_preset());
                lfo.set_preset(preset.get_lfo_preset());
                mod.set_preset(preset.get_mod_preset());

                //leds.restore_state();
		    }
		    last_disp_count = 0;
		}

        last_disp_pushed = pushed;

		mod.update();
	}
}

