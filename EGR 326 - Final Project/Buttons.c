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
// Setting data direction for buttons as inputs (already set to zero)
BUTTON_DD_1 &= ~_BV(BUTTON_1_PIN);
BUTTON_DD_2 &= ~_BV(BUTTON_2_PIN);
BUTTON_DD_3 &= ~_BV(BUTTON_3_PIN);

// Enabling internal pull-up resistor on pins attached to buttons
BUTTON_1_PORT |= _BV(BUTTON_1_PIN);     
BUTTON_2_PORT |= _BV(BUTTON_2_PIN);
BUTTON_3_PORT |= _BV(BUTTON_3_PIN);

}