
#include "mbed.h"
#include "src/Mux.h"
#include "src/Pwm.h"
#include "src/Knob.h"
#include "src/UI_BLOCK.h"
#include "src/ADSR.h"

#define MUX_COUNT 2

PinName mux_addr[MUX_COUNT] = {PA_0, PA_1};

int main() {
	Pwm leds;
	Mux mux[MUX_COUNT];
	ADSR adsr;

	leds.init();

	for(int i=0; i < MUX_COUNT; i++) {
		mux[i].init(mux_addr[i]);
	}

	adsr.init(mux, &leds);

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux[i].update();
		}
		//mux.print();
		adsr.update();
	}
}

