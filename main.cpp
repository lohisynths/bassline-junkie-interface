#include "mbed.h"
#include "PCA9685.h"
#include "USBMIDI.h"

DigitalOut led(LED1);


void show_message(MIDIMessage msg) {
    switch (msg.type()) {
        case MIDIMessage::NoteOnType:
            printf("NoteOn key:%d, velocity: %d, channel: %d\n", msg.key(), msg.velocity(), msg.channel());
            break;
        case MIDIMessage::NoteOffType:
            printf("NoteOff key:%d, velocity: %d, channel: %d\n", msg.key(), msg.velocity(), msg.channel());
            break;
        case MIDIMessage::ControlChangeType:
            printf("ControlChange controller: %d, data: %d\n", msg.controller(), msg.value());
            break;
        case MIDIMessage::PitchWheelType:
            printf("PitchWheel channel: %d, pitch: %d\n", msg.channel(), msg.pitch());
            break;
        default:
            printf("Another message\n");
    }
}

USBMIDI midi;
PCA9685 pwm(I2C_SDA, I2C_SCL);
bool toggle=0;

int main() {
	pwm.begin();
    midi.attach(show_message);         // call back for messages received
    while (1) {
        for(int i=48; i<83; i++) {     // send some messages!
            midi.write(MIDIMessage::NoteOn(i));
            led = !led;
            wait(0.25);
            led = !led;
            midi.write(MIDIMessage::NoteOff(i));
            wait(0.5);
            pwm.setPWM(0,0, toggle*256);
        	toggle^=1;
        }
    }
}
