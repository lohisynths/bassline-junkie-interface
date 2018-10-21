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
#include "blocks/VOL.h"

#include "ui_config.h"

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
	USBMIDI midi_usb;
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
	midi_usb.attach(do_message);
	midi_glob = &midi;
	leds.init();
	mux.init();
    preset.load_global(0);

	adsr.init(adsr_knob_config, adsr_button_config, &midi, &leds, &mux);
	osc.init(osc_knob_ctrl, osc_button_ctrl, &midi, &leds, &mux);
    filter.init(flt_knob_ctrl, flt_button_ctrl, &midi, &leds, &mux);
	mod.init(mod_knob_ctrl, mod_button_ctrl, &midi, &leds, &mux);
	lfo.init(lfo_knob_ctrl, lfo_button_ctrl, &midi, &leds, &mux);
	display.init(disp_knob_ctrl, disp_button_ctrl, &midi, &leds, &mux);
	vol.init(vol_knob_ctrl, vol_button_ctrl, &midi, &leds, &mux);

    preset.init(&adsr, &osc, &lfo, &mod, &filter, &vol);
    preset.update_preset();

    bool mod_viewer_mode = false;

	while(1) {
		mux.update();
		vol.update();
		osc.update();
        adsr.update();
        lfo.update();
        filter.update();
        display.update();
        mod.update();

		int ret = -1;

		ret = osc.get_first_knob_sw_pushed();
		if(ret > -1) {
            mod.select_MOD_dest(ret+(osc.get_current_osc()*5));
		}

        ret = filter.get_first_knob_sw_pushed();
        if(ret > -1) {
            mod.select_MOD_dest(ret+15);
		}

        ret = display.preset_changed();
        if(ret > -1) {
            LOG::LOG1("loading preset %d\r\n", ret);
            preset.load_preset_eeprom(ret);
            preset.update_preset();
            LOG::LOG1("preset %d loaded\r\n", ret);
        }

        ret = display.get_long_push();
        if(ret == 1) {
            LOG::LOG1("saving preset %d\r\n", display.get_actual_preset_nr());
            preset.save_preset_eeprom(display.get_actual_preset_nr());
            LOG::LOG1("preset %d saved\r\n", display.get_actual_preset_nr());
        }

        ret = mod.get_first_knob_sw_pushed();
        if(ret > -1) {
            mod_viewer_mode ^= 1;
            osc.set_viewer_mode(mod_viewer_mode);
            LOG::LOG1("mod_viever mode changed %d\r\n", mod_viewer_mode);
        }
	}
}

