/*
 * Timers.h
 *
 * Created: 10/13/2014 1:24:07 PM
 *  Author: Jonathan
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init();
void hw_delay(uint16_t delay_ms);
void Timer1PWM_OnOff(uint8_t state);





#endif /* TIMERS_H_ */