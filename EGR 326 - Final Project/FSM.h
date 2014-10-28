/*
 * FSM.h
 *
 * Created: 10/16/2014 3:12:56 AM
 *  Author: Jonathan
 */ 
#include <inttypes.h>

#ifndef FSM_H_
#define FSM_H_
// Defining main states
#define ALARM_DISPLAY 1
#define TIME_DISPLAY  2
#define RADIO_DISPLAY 3

#ifndef STATE
#define STATE

typedef struct{
	uint8_t previous,
			current,
			next;
			
}state_t;

#endif

state_t determine_state(state_t state, int button_pressed);




#endif /* FSM_H_ */