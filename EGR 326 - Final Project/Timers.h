/*
 * Timers.h
 *
 * Created: 10/13/2014 1:24:07 PM
 *  Author: Jonathan
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

//Function that initializes Timer 1 for 1kHz interrupt used for
//button polling
void Timer0_Init(void);
//Function that initializes Timer 1 for a PWM frequency to run solenoid
void Timer1_Init(void);
//Function that initializes Timer 2 in CTC mode for a hw delay
void Timer2_Init(void);
//HW delay function that takes in time to delay in ms
void hw_delay(uint16_t delay_ms);
//Function that turns the PWM on and off
void Timer1PWM_OnOff(uint8_t state);





#endif /* TIMERS_H_ */