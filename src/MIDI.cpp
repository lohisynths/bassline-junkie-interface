/*
 * MIDI.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: alax
 */

#include "MIDI.h"
#include "utils.h"



MIDI::MIDI() : pc(PA_9, PA_10, 115200) {}

MIDI::~MIDI() {}

ssize_t MIDI::raspi_usart_write(const void* buffer, size_t length) {
    const char* ptr = (const char*)buffer;
    const char* end = ptr + length;

    while (ptr != end) {
        if (pc.putc(*ptr++) == EOF) {
            break;
        }
    }
    return ptr - (const char*)buffer;
}

void MIDI::send_cc(uint8_t CC, uint8_t val, uint8_t channel) {
	MIDIMessage msg = MIDIMessage::ControlChange(CC, val, channel);
	raspi_usart_write(msg.data, msg.length);
	show_message(msg);
}


void MIDI::show_message(MIDIMessage msg) {
	switch (msg.type()) {
	case MIDIMessage::NoteOnType:
		DEBUG_LOG("NoteOn key:%d, velocity: %d, channel: %d\r\n", msg.key(),
				msg.velocity(), msg.channel());
		break;
	case MIDIMessage::NoteOffType:
		DEBUG_LOG("NoteOff key:%d, velocity: %d, channel: %d\r\n", msg.key(),
				msg.velocity(), msg.channel());
		break;
	case MIDIMessage::ControlChangeType:
		DEBUG_LOG("ControlChange controller: %d, data: %d\r\n", msg.controller(),
				msg.value());
		break;
	case MIDIMessage::PitchWheelType:
		DEBUG_LOG("PitchWheel channel: %d, pitch: %d\r\n", msg.channel(),
				msg.pitch());
		break;
	default:
		printf("Another message\r\n");
	}
}
