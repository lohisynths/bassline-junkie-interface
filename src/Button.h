/*! \file Button.h
 *  \brief Header file for Button class.
 *  \author lohi-synths
 *  \version 0.0
 *  \date Nov 1 2017
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#include "mbed.h"
#include "Pwm.h"
#include "Mux.h"
#include "utils.h"

/*! \class Button
 *  \brief Button class - Interface for illuminated switches.
 *
 *   Button means real user interface block, consisting of physical
 *   illuminated switch on PCB.<br>
 *   This class is an interface for reading button data from Mux
 *   and controlling integrated illumination with Pwm.
 */
class Button {
public:

	/*! \struct button_init_map
	 *  \brief Button Initialization struct
	 */
	struct button_init_map {
		                                      /*!  \name Mux object related data */
		Mux *mux;                             /*!< Pointer to global Mux object */
		uint16_t *mux_raw_data;               /*!< Pointer to Button specific Mux raw data */
		uint8_t mux_first_bit;                /*!< First bit in Button specific Mux raw data */
		                                      /*!  \name Pwm object related data */
		Pwm *leds;					          /*!< Pointer to global Pwm object */
		uint16_t max_led_value;		          /*!< Led maximum brightness <br>
									           *   PCA9685 12 bit pwm -> 4095 values */
		uint8_t first_pwm_output;             /*!< Address of len in Pwm leds array <br>
		                                       *   (0 - PWM_DRIVERS_COUNT * 16) */
	};

	/*! \fn Button()
	 *  \brief Empty constructor
	 */
	Button() {};

	/*! \fn ~Button()
	 *  \brief Empty destructor
	 */
	virtual ~Button() {};

	/*! \fn bool init(button_init_map button_configuration)
	 *  \brief Initialize object.
	 *
	 *   Initialize object with button_init_map structure.<br>
	 *   This function needs to be called just after startup.
	 *
	 *  \warning No null checking in runtime code!<br>
	 *   this->button_config must be properly initialized with button_configuration
	 *	 \param[in]     button_configuration Initialization struct
	 */
	void init(button_init_map button_configuration);

	/*! \fn bool update()
	 *  \brief Update current state of Button.
	 *
	 *   Read button state from Pwm register.<br>
	 *   If switch state changed return true, otherwise false.
	 *
	 *  \return True when switch value changed.
	 *  \sa get_state()
	 */
	bool update();

	/*! \fn bool get_state()
	 *  \brief Get last state of Button.
	 *
	 *   Return last known state of switch.<br>
	 *	 This function does not fetch switch data from Mux.<br>
	 *	 Just returns Knob::last_state value
	 *
	 *  \return Last state of switch.
	 *  \sa update()
	 */
	bool get_state();

	/*! \fn void set_led_val(uint16_t brightness)
	 *  \brief Set value of switch illumination brightness.
	 *
	 *   Set value of pulse width modulation on corresponding to Button output of Pwm.<br>
	 *	 Maximum input value = 4095 (PCA9685 12 bit pwm)<br>
	 *	\param[in] brightness Switch illumination brightness (0-4095)
	 */
	void set_led_val(uint16_t brightness);

	/*! \fn button_init_map &get_config()
	 *  \brief Get reference to current Knob::button_config.
	 *
	 *  \return Reference to current Knob::button_config.
	 */
	button_init_map &get_config() {
		return button_config;
	}

private:
	/*! \var button_init_map button_config
	 *  \brief Button configuration structure.
	 */
	button_init_map button_config;

	/*! \var uint8_t last_state
 	 *  \brief Last switch state.
 	 *
 	 *   Upadted Knob::update() function.
	 */
	uint8_t last_state=1;
	void print_config(button_init_map config);
};

#endif /* SRC_BUTTON_H_ */
