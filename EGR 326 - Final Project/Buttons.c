 /*
 * Buttons.c
 *
 * Created: 10/11/2014 3:54:30 PM
 *  Author: Jonathan
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "Buttons.h"

/*--------------------------------------------------------------------------------------------------
Name : BUTTON_init
Description : Sets data directions for buttons, LEDs, and switches as well as enables internal pull-ups
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void BUTTON_init()
{
	//Note: Still need to add switches and LEDs
// Setting data direction for buttons and switches as inputs (already set to zero)
BUTTON_DD_1 &= ~_BV(BUTTON_1);
BUTTON_DD_2 &= ~_BV(BUTTON_2);
BUTTON_DD_3 &= ~_BV(BUTTON_3);
SWITCH_DD_1 &= ~_BV(SWITCH_1);
SWITCH_DD_2 &= ~_BV(SWITCH_2);

// Enabling internal pull-up resistor on pins attached to buttons and switches
BUTTONS_PORT |= _BV(BUTTON_1);     
BUTTONS_PORT |= _BV(BUTTON_2);
BUTTONS_PORT |= _BV(BUTTON_3);
BUTTONS_PORT |= _BV(SWITCH_1);
BUTTONS_PORT |= _BV(SWITCH_2);

//Setting data directions for LEDs
DDRC |= _BV(LED_RADIO);
DDRC |= _BV(LED_STEREO);

//Turn LEDs off initially
LED_RADIO_OFF;
LED_STEREO_OFF;

}