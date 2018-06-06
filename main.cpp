
#include "mbed.h"
#include "src/Mux.h"
#include "src/Pwm.h"
#include "src/Knob.h"
#include "src/UI_BLOCK.h"
#include "src/ADSR.h"
#include "src/OSC.h"
#include "src/MOD.h"



int main() {
	Pwm leds;
	Mux mux;
	ADSR adsr;
	OSC osc;
	MOD mod;

	leds.init();
	mux.init();
	adsr.init(&mux, &leds);
	osc.init(&mux, &leds);
	mod.init(&mux, &leds);

	bool rap=false;

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}
		//mux.print_bit(3, 2);
		//mux.print(2);

		adsr.update();
		osc.update();
		mod.update();
	}
}

