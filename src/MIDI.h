/*
 * MIDI.h
 *
 *  Created on: Jun 6, 2018
 *      Author: alax
 */

#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

#include "mbed.h"
#include "USBDevice/USBMIDI/MIDIMessage.h"


class MIDI {
public:
	MIDI();
	virtual ~MIDI();

	void send_cc(uint8_t CC, uint8_t val, uint8_t channel);
	void send_note(uint8_t key, uint8_t velocity, uint8_t channel);
	void putc(uint8_t key);

private:
	Serial pc;

	ssize_t raspi_usart_write(const void* buffer, size_t length);
	void show_message(MIDIMessage msg);

};

#endif /* SRC_MIDI_H_ */
