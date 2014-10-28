/*
 * FSM.c
 *
 * Created: 10/16/2014 3:13:11 AM
 *  Author: Jonathan
 */ 
#include "FSM.h"
#include "Buttons.h"

state_t determine_state(state_t state, int button_pressed)
{
	uint8_t temp_state;
	
	if(button_pressed == BACK_BUTTON)
		{
			temp_state = state.current;
			state.current = state.previous;
			state.previous = temp_state;
		}
		else if(button_pressed == SETTING_BUTTON)
			{
			// do nothing for now
			}
			else if(button_pressed == FORWARD_BUTTON)
				{
			     temp_state = state.current;
				 state.current = state.next;
				 state.next = temp_state;
				}
		return(state);
}