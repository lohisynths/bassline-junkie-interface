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

    /*! \var const char *name
     *  \brief const char * defining class name
     */
    const char *name = "Knob";
    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<KNOB_LOG_LEVEL> LOG;

	/*! \struct knob_init_map
	 *  \brief Knob Initialization structure
	 */
	struct knob_init_map {
		/*!  \name Mux object related data */
        uint16_t mux_number;                  /*!< Mux number in global Mux array. */
		uint8_t mux_first_bit;                /*!< First bit in Knob specific Mux raw data. */
		uint16_t encoder_max_value;           /*!< Maximum encoder value.
		 	 	 	 	 	 	 	 	 	   *   Internal value of encoder TODO:  */
		/*!  \name Pwm object related data. */
		uint16_t max_led_value;      		  /*!< Led maximum brightness \n\n
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
	void init(knob_init_map config, Pwm *_leds, Mux *_mux);

	/*! \fn knob_msg update()
	 *  \brief Update current state of Knob.
	 *
	 *   Read encoder data from Pwm register.\n\n
	 *   If switch state changed return true, otherwise false.
	 *
	 *  \return
	 *   knob_msg.value_changed == true if rotary encoder value changed\n
	 *   knob_msg.switch_changed == true if rotary encoder switch state changed.
	 */
	knob_msg update();

	void led_indicator_set_value(uint16_t value, bool force  = false);
	void set_value(uint16_t val , bool force_led_update = false);
	uint16_t get_knob_value();
	bool get_switch_state();

private:

	Pwm *leds = nullptr;
    Mux *mux = nullptr;

	/*! \var knob_init_map knob_config
	 *  \brief Knob configuration structure.
	 */
	knob_init_map knob_config={};

	/*! \fn uint8_t calculate_led_position(uint8_t led_number)
	 *  \brief Calculate absolute position of led on scale illumination.
	 *
	 *   Leds in PCA9685 address space are located counter-clockwise.\n\n
	 *   Therefore we subtract (knob_config.total_led_count - 1) and
	 *   add knob_config.first_pwm_output to get absolute address of input led.
	 *
	 * 	\param[in] led_number Number of led corresponding to encoder calie.
	 *
	 *  \return
	 *   knob_msg.value_changed == true if rotary encoder value changed\n
	 *   knob_msg.switch_changed == true if rotary encoder switch state changed.
	 */
	uint8_t calculate_led_position(uint8_t led_number);



	void led_on_last_off(uint8_t led_on_nr, uint8_t led_off_nr, int16_t bright);

	void print_config(knob_init_map config);

	/*! \var Enc encoder
	 *  \brief Enc class, to for gray code decoding.
	 */
	Enc encoder;

	/*! \var uint8_t actual_value.
	 *  \brief Knob actual value.\n
	 *   value in range from 0 to maximum value knob_init_map.encoder_max_value
	 */
	uint8_t actual_value = 0;

	/*! \var uint8_t last_value.
	 *  \brief Knob last value.\n
	 *   value in range from 0 to maximum value knob_init_map.encoder_max_value
	 */
	int16_t last_enc_value = 0;
	uint16_t last_led_on = 0;
	uint16_t last_sw_state = 1;

	float encoder_max_to_127_divider = 0;
	float encoder_2_midi_mult = 0;
};

#endif /* SRC_KNOB_H_ */
