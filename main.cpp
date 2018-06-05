
#include "src/Synth.h"

// PA_0 - mux 0 - env encoders, env buttons
// 0-2  - env encoder 0
// 3-5  - env encoder 1
// 6-8  - env encoder 2
// 9-11 - env encoder 3
// 12   - env button  loop
// 13   - env button  2
// 14   - env button  1
// 15   - env button  0


uint16_t mux_data;

Mux mux(PA_0);

Pwm leds;
Knob knob;
uint8_t last_led;

ADSR_CTL adsr;


int main() {
	leds.init();

	adsr.init(leds, mux_data);



	while(1) {
		mux_data = mux.get();
		adsr.update();
	}


	//Synth synth;


	//synth.run();


}

