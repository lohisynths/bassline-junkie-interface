/*
 * MIDI.h
 *
 *  Created on: Jun 6, 2018
 *      Author: alax
 */

#ifndef SRC_MIDI_H_
#define SRC_MIDI_H_

#include "mbed.h"
#include "MIDIMessage.h"
#include "utils.h"

class MIDI {
public:

    /*! \var const char *name
     *  \brief const char * defining class name
     */
    const char *name = "MIDI";
    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<MIDI_LOG_LEVEL> LOG;

    MIDI() : pc(PA_9, PA_10, 115200) {}
    virtual ~MIDI() {}

	void send_cc(uint8_t CC, uint8_t val, uint8_t channel);
	void send_note_off(uint8_t key, uint8_t velocity, uint8_t channel);
	void send_note_on(uint8_t key, uint8_t velocity, uint8_t channel);
    void show_message(MIDIMessage msg);

private:
	Serial pc;
	ssize_t raspi_usart_write(const void* buffer, uint8_t length);
};

#endif /* SRC_MIDI_H_ */
