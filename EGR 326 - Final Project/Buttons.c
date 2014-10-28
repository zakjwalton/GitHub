/*
 * Buttons.c
 *
 * Created: 10/11/2014 3:54:30 PM
 *  Author: Jonathan
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "Buttons.h"

void BUTTON_init()
{
// Setting data direction for buttons as inputs (already set to zero)

// Setting pins that buttons are attached to as input capture pins (generates an input when any of the three pins are pressed)

// Enabling internal pull-up resistor on pins attached to buttons
BUTTON_1_PORT |= _BV(BUTTON_1_PIN);     
BUTTON_2_PORT |= _BV(BUTTON_2_PIN);
BUTTON_3_PORT |= _BV(BUTTON_3_PIN);

// Enabling Pin change interrupts on pins PC0-PC7
//PCICR |= _BV(PCIE0);

// Enabling the three buttons used for the alarm clock
//PCMSK1 |= _BV(PCINT10) | _BV(PCINT9) | _BV(PCINT8);
}

/*************************************************
Function that debounces a button
*************************************************/

int debounce(int Button, int Button_Port)
{
	static uint16_t State = 0; //Current debounce status
	//read switch, upper 5 bits of State are don't cares
	State = State<<1 | (Button_Port & _BV(Button))>>6 | 0xf800;
	//indicate 0 level for 10 consecutive calls
	if(State == 0xfc00){
		return 1;
	}
	else
	return(0);
}

int get_new_button()
{	
	// Check to see which button was pressed
	
	//Checking if B1 is pressed
	if(!(PINC & _BV(PC0)))
	{
		return(BACK_BUTTON);
	}
	else if(!(PINC & _BV(PC1)))
	{
		return(SETTING_BUTTON);
	}
	else if(!(PINC & _BV(PC2)))
	{
		return(FORWARD_BUTTON);
	}
	else
	{
		return(0);
	}
}