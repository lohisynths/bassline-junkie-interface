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

void MIDI::putc(uint8_t key) {
	pc.putc(key);
   	DEBUG_LOG("putc %d\r\n", key);
	//show_message(msg);
}

void MIDI::send_note_on(uint8_t key, uint8_t velocity, uint8_t channel) {
	uint8_t msg[3];
    msg[0] = 0x90 | (channel & 0x0F);
    msg[1] = key & 0x7F;
    msg[2] = velocity & 0x7F;

   	raspi_usart_write(msg, sizeof(msg));
   	DEBUG_LOG("NOTE_ON :%d, velocity: %d, \r\n", key, velocity);
	//show_message(msg);
}

void MIDI::send_note_off(uint8_t key, uint8_t velocity, uint8_t channel) {
	uint8_t msg[3];
    msg[0] = 0x80 | (channel & 0x0F);
    msg[1] = key & 0x7F;
    msg[2] = velocity & 0x7F;

   	raspi_usart_write(msg, sizeof(msg));
   	DEBUG_LOG("NOTE_OFF :%d, velocity: %d, \r\n", key, velocity);
	//show_message(msg);
}

void MIDI::send_cc(uint8_t control, uint8_t value, uint8_t channel) {
	uint8_t msg[3];
    msg[0] = 0xB0 | (channel & 0x0F);
    msg[1] = control & 0x7F;
    msg[2] = value & 0x7F;

   	raspi_usart_write(msg, sizeof(msg));
	//DEBUG_LOG("CC :%d, velocity: %d, \r\n", control, value);
	//show_message(msg);
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
