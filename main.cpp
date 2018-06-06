
#include "mbed.h"
#include "src/Mux.h"
#include "src/Pwm.h"
#include "src/Knob.h"

#include "src/blocks/ADSR.h"
#include "src/blocks/OSC.h"
#include "src/blocks/MOD.h"
#include "src/blocks/LFO.h"



int main() {
	Pwm leds;
	Mux mux;
	ADSR adsr;
	OSC osc;
	MOD mod;
	LFO lfo;

	leds.init();
	mux.init();
//	adsr.init(&mux, &leds);
//	osc.init(&mux, &leds);
//	mod.init(&mux, &leds);
	lfo.init(&mux, &leds);

	bool rap=false;

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}
		static int j=0;
		if(!rap) {
			j++;
			if(j>32) j=0;
		}


//		leds.set(8*16+j, (rap^=1)*1024);
		//mux.print_bit(4, 0);
		//mux.print(4);

	//	adsr.update();
	//	osc.update();
	//	mod.update();
		lfo.update();
	}
}

