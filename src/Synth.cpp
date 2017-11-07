/*
 * Synth.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: alax
 */

#include "Synth.h"

	uint8_t notes[] = { 42, 44, 41 };
	static uint8_t seq = 0;

Synth::Synth() : led(LED1), mux_enc(1), mux_sw(0) {

	leds.init();


	adsr.init(&leds, mux_enc.get(), mux_sw.get(), &midi);
	osc.init(&leds, mux_enc.get(), mux_sw.get(), &midi);

	printf("configured = %d \n", midi.configured());
}

Synth::~Synth() {
	// TODO Auto-generated destructor stub
}

void Synth::run()
{
	while (1) {
		static uint16_t note_on_loop=0;
		const uint16_t loop_count = 5000;

		if (note_on_loop++ > loop_count && start) {
			led = led ^ 1;
			note_on_loop = 0;
			seq++;
			if (seq > 2)
				seq = 0;

			midi.write(MIDIMessage::NoteOn(notes[seq]));

			last_note = notes[seq];
			last_note_off = false;
		}
		if (note_on_loop > loop_count / 8 && last_note_off == false) {
			led = led ^ 1;

			if (!start)
				note_on_loop = 0;
			last_note_off = true;

			midi.write(MIDIMessage::NoteOff(last_note));
		}

		uint32_t mux_data = mux_enc.get();
		uint32_t sw_data = mux_sw.get();
		enc8.update(mux_data, sw_data);

		adsr.update(mux_data, sw_data);
		osc.update(mux_data, sw_data);
	}
}



//midi.attach(show_message);         // call back for messages received

void show_message(MIDIMessage msg) {
	switch (msg.type()) {
	case MIDIMessage::NoteOnType:
		printf("NoteOn key:%d, velocity: %d, channel: %d\n", msg.key(),
				msg.velocity(), msg.channel());
		break;
	case MIDIMessage::NoteOffType:
		printf("NoteOff key:%d, velocity: %d, channel: %d\n", msg.key(),
				msg.velocity(), msg.channel());
		break;
	case MIDIMessage::ControlChangeType:
		printf("ControlChange controller: %d, data: %d\n", msg.controller(),
				msg.value());
		break;
	case MIDIMessage::PitchWheelType:
		printf("PitchWheel channel: %d, pitch: %d\n", msg.channel(),
				msg.pitch());
		break;
	default:
		printf("Another message\n");
	}
}
