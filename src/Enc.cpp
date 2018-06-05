/*
 * Enc.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: alax
 */

#include "Enc.h"

Enc::Enc(){

}

Enc::~Enc() {
	// TODO Auto-generated destructor stub
}

void Enc::init(uint16_t &data, uint8_t first_bit) {
	mux_data = &data;
	bit_a = first_bit;
	lastModeA = 0;
	lastModeB = 0;
	encPos = 0;
	encPosLast = 0;
}


void Enc::set(int16_t val){
	encPos=val;
};

int16_t Enc::get(){
	return encPos;
};




// https://www.allaboutcircuits.com/projects/how-to-use-a-rotary-encoder-in-a-mcu-based-project/
bool Enc::update() {
	curModeA = CHECKBIT(*mux_data, bit_a+1);
	curModeB = CHECKBIT(*mux_data, bit_a);
	// utility variables
	bool change=0;
	// compare the four possible states to figure out what has happened
	//   then encrement/decrement the current encoder's position
	if (curModeA != lastModeA) {
		if (curModeA == false) {
			if (curModeB == false) {
				encPos--;
			} else {
				encPos++;
			}
		} else {
			if (curModeB == false) {
				encPos++;
			} else {
				encPos--;
			}
		}
	}
	if (curModeB != lastModeB) {
		if (curModeB == false) {
			if (curModeA == false) {
				encPos++;
			} else {
				encPos--;
			}
		} else {
			if (curModeA == false) {
				encPos--;
			} else {
				encPos++;
			}
		}
	}
	// set the current pin modes (HIGH/LOW) to be the last know pin modes
	//   for the next loop to compare to
	lastModeA = curModeA;
	lastModeB = curModeB;
	// if this encoder's position changed, flag the change variable so we
	//   know about it later
	if (encPos != encPosLast) {
		change = 1;
	}

	if (change == 1) {
		// if an encoder has changed, do something with that information
		// here, I am just going to print all the encoder's positions
		//   if any of them change
		//   Serial.print("  Position");
		//    Serial.print(1);
		//    Serial.print(": ");
		//   Serial.print(encPos);
		encPosLast = encPos;
		//  Serial.println();
		// debounce - if there has been a change, wait for a bit (so to speak) to let the
		//   bounces settle - change to your liking
	}
	return change;
}
