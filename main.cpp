#include "mbed.h"
#include "PCA9685.h"
#include "USBMIDI.h"
#include "src/Pwm.h"
#include "src/Mux.h"
#include "src/ADSRCTL.h"
#include "src/OSCCTL.h"
#include "src/ENC8.h"

DigitalOut led(LED1);

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

bool toggle = 0;
bool start = true;

int i = 0;
uint8_t notes[] = { 42, 44, 41 };
static uint8_t seq = 0;
int loop_count = 5000;
uint8_t last_note = 0;
bool last_note_off = true;

int main() {

	USBMIDI midi;

	Mux mux_enc(1);
	Mux mux_sw(0);
	Pwm leds;

	leds.init();

	ADSR_CTL adsr;
	OSC_CTL osc;
	ENC8 enc8;

	adsr.init(&leds, mux_enc.get(), mux_sw.get(), &midi);
	osc.init(&leds, mux_enc.get(), mux_sw.get(), &midi);

	printf("configured = %d \n", midi.configured());

	midi.attach(show_message);         // call back for messages received
	while (1) {

		if (i++ > loop_count && start) {
			led = led ^ 1;
			i = 0;
			seq++;
			if (seq > 2)
				seq = 0;

			midi.write(MIDIMessage::NoteOn(notes[seq]));

			last_note = notes[seq];
			last_note_off = false;
		}
		if (i > loop_count / 8 && last_note_off == false) {
			led = led ^ 1;

			if (!start)
				i = 0;
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
