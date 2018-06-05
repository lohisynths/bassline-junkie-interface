
#include "mbed.h"
#include "src/Mux.h"
#include "src/Pwm.h"
#include "src/Knob.h"
#include "src/UI_BLOCK.h"
#include "src/ADSR.h"



int main() {
	Pwm leds;
	Mux mux;
	ADSR adsr;

	leds.init();
	mux.init();
	adsr.init(&mux, &leds);

	while(1) {
		for(int i=0; i < MUX_COUNT; i++) {
			mux.update();
		}
		//mux.print(1);
		adsr.update();
	}
}

