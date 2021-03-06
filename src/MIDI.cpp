/*
 * MIDI.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: alax
 */

#include "MIDI.h"

ssize_t MIDI::raspi_usart_write(const void* buffer, uint8_t length) {
    const uint8_t* ptr = (const uint8_t*)buffer;
    const uint8_t* end = ptr + length;

    while (ptr != end) {
    	uint8_t data = *ptr++;
        if (pc.putc(data) == EOF) {
            break;
        }
    }
    return ptr - (const uint8_t*)buffer;
}

void MIDI::send_note_on(uint8_t key, uint8_t velocity, uint8_t channel) {
    LOG::LOG0("%s NoteOn key:%d, velocity: %d, channel: %d\r\n", name, key, velocity, channel);
	uint8_t msg[3];
    msg[0] = 0x90 | (channel & 0x0F);
    msg[1] = key & 0x7F;
    msg[2] = velocity & 0x7F;
   	raspi_usart_write(msg, sizeof(msg));
}

void MIDI::send_note_off(uint8_t key, uint8_t velocity, uint8_t channel) {
    LOG::LOG0("%s NoteOff key:%d, velocity: %d, channel: %d\r\n", name, key, velocity, channel);
	uint8_t msg[3];
    msg[0] = 0x80 | (channel & 0x0F);
    msg[1] = key & 0x7F;
    msg[2] = velocity & 0x7F;
   	raspi_usart_write(msg, sizeof(msg));
}

void MIDI::send_cc(uint8_t control, uint8_t value, uint8_t channel) {
    LOG::LOG1("%s ControlChange %d channel %d data %d\r\n", name, control, channel, value);
	uint8_t msg[3];
    msg[0] = 0xB0 | (channel & 0x0F);
    msg[1] = control & 0x7F;
    msg[2] = value & 0x7F;
   	raspi_usart_write(msg, sizeof(msg));
}

void MIDI::show_message(MIDIMessage msg) {
	switch (msg.type()) {
	case MIDIMessage::NoteOnType:
	    LOG::LOG2("%s NoteOn key:%d, velocity: %d, channel: %d\r\n", name, msg.key(), msg.velocity(), msg.channel());
		break;
	case MIDIMessage::NoteOffType:
	    LOG::LOG2("%s NoteOff key:%d, velocity: %d, channel: %d\r\n", name, msg.key(), msg.velocity(), msg.channel());
		break;
	case MIDIMessage::ControlChangeType:
	    LOG::LOG2("%s ControlChange controller: %d, data: %d\r\n", name, msg.controller(), msg.value());
		break;
	case MIDIMessage::PitchWheelType:
	    LOG::LOG2("%s PitchWheel channel: %d, pitch: %d\r\n", name, msg.channel(), msg.pitch());
		break;
	default:
	    LOG::LOG2("Another message\r\n");
	}
}
