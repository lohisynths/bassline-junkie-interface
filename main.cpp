
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


int main() {
	leds.init();
	knob.init(10, 3, leds, mux_data);

	while(1) {
		mux_data = mux.get();
		//mux.print();
		knob_msg msg = knob.update();
		if(msg.switch_changed) {
			printf("%d\r\n",knob.get_sw_state() );
		}
		if(msg.value_changed) {
			int led_nr = knob.get_value() / 7;
			if(led_nr != last_led) {
				printf("led_nr %d last_led %d\r\n",led_nr, last_led );
				knob.led_on(led_nr, 1024);
				last_led = led_nr;
			}
			printf("%d\r\n",knob.get_value() );
		}
	}


	//Synth synth;


	//synth.run();


}

