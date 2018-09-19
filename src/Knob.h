/*! \file Knob.h
 *  \brief Header file for Knob class.
 *  \author lohi-synths
 *  \version 0.0
 *  \date Nov 1 2017
 */

#ifndef SRC_KNOB_H_
#define SRC_KNOB_H_

#include "mbed.h"
#include "Pwm.h"
#include "Mux.h"
#include "Enc.h"

/*! \class Knob
 *  \brief Knob class - Interface for rotary encoder with illuminated led scale.
 *
 *   Knob is class representing physical interface block, consists of
 *   rotary encoder and leds placed around it.\n\n
 *   This class is an interface for reading encoder data from Mux
 *   and controlling illuminated led scale with Pwm.
 */
class Knob {
public:

	/*! \struct knob_init_map
	 *  \brief Knob Initialization structure
	 */
	struct knob_init_map {
								              /*!  \name Mux object related data */
		Mux *mux;                             /*!< Pointer to global Mux object. */
		uint16_t *mux_raw_data;               /*!< Pointer to Knob specific Mux raw data. */
		uint8_t mux_first_bit;                /*!< First bit in Knob specific Mux raw data. */
		uint16_t encoder_max_value;           /*!< Maximum encoder value.
		 	 	 	 	 	 	 	 	 	   *   Internal value of encoder TODO:  */
		                                      /*!  \name Pwm object related data. */

		Pwm *leds;                 		      /*!< Pointer to global Pwm object.
                                               *   Initialized in init() function. */
		uint8_t max_led_value;      		  /*!< Led maximum brightness \n\n
                                               *   PCA9685 12 bit pwm -> 4095 values. */
		uint8_t first_pwm_output;             /*!< Address of led in Pwm leds array.
                                               *   Maximum value = (PWM_DRIVERS_COUNT * 16) */
		uint8_t total_led_count;    		  /*!  Total led count (Knob only)*/
	};

	/*! \struct knob_msg
	 *  \brief Knob update() return structure type
	 */
	struct knob_msg{
		/*! \var bool value_changed
		 *  \brief Changed if rotary encoder value gets changed.
		 */
		bool value_changed=false;
		/*! \var bool switch_changed
		 *  \brief Changed if rotary encoder switch state gets changed.
		 */
		bool switch_changed = false;
	};

	/*! \fn Knob()
	 *  \brief Empty constructor
	 */
	Knob() {};

	/*! \fn ~Knob()
	 *  \brief Empty destructor
	 */
	virtual ~Knob() {};

	/*! \fn void init(knob_init_map config)
	 *  \brief Initialize Knob object.
	 *
	 *   Initialize object with button_init_map structure.\n\n
	 *   This function needs to be called just after startup.
	 *
	 *  \warning No null checking in runtime code!
	 *   this->button_config must be properly initialized with
	 *   proper filled Knob::button_configuration structure.
	 *	 \param[in] config Initialization structure.
	 */
	void init(knob_init_map config);

	/*! \fn knob_msg update()
	 *  \brief Update current state of Knob.
	 *
	 *   Read encoder data fron Pwm register.\n\n
	 *   If switch state changed return true, otherwise false.
	 *
	 *  \return
	 *   knob_msg.value_changed == true if rotary encoder value changed\n
	 *   knob_msg.switch_changed == true if rotary encoder switch state changed.
	 */
	knob_msg update();

	void set_leds(uint16_t value);
	void set_value(uint16_t val);
	uint16_t get_knob_value();
	bool get_switch_state();
	knob_init_map knob_config={};

private:
	void led_on_last_off(size_t led_nr, int16_t bright);
	void print_config(knob_init_map config);

	Enc encoder;

	uint16_t encoder_value = 0;
	int16_t encoder_value_last = 0;
	uint16_t led_last = 0;
	uint16_t last_sw_state = 1;

	float encoder_max_to_127_divider = 0;
	float encoder_2_midi_mult = 0;
};

#endif /* SRC_KNOB_H_ */
