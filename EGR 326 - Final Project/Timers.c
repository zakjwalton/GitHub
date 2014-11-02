/*
 * Timers.c
 *
 * Created: 10/13/2014 1:23:56 PM
 *  Author: Jonathan
 */ 

#include <avr/io.h>

/*--------------------------------------------------------------------------------------------------
Name : Timer0_Init
Description : Sets up Timer 0 to interrupt every ms for button polling
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void Timer0_Init(void){
	//Setup timer 0 w/ interrupt every ms and pre-scale of 64
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(OCIE0A);
	OCR0A = 250;
	TIFR0 |= _BV(OCF0A);
	TCNT0 = 0;
}

/*--------------------------------------------------------------------------------------------------
Name : Timer2_Init
Description : Sets up Timer 2 to be used for a simple HW delay with 1024 prescalar
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void Timer2_Init(void){
	//Setup timer 1 to run in CTC mode 4 with 1024 prescalar
	TCCR2A = _BV(WGM21);
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
	
}

/*--------------------------------------------------------------------------------------------------
Name : Timer1_Init
Description : Function that initializes Timer 1 in PWM mode @20Hz with a 25% duty cycle for solenoid alarm
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/
void Timer1_Init(void)
{
	DDRB |= _BV(PB1);
	TCCR1A = _BV(COM1A1);
	TCCR1B = _BV(WGM13);
	ICR1 = 390;
	OCR1A = 90;
}

/*--------------------------------------------------------------------------------------------------
Name : hw_delay
Description : Function that uses Timer 2 to implement specified HW delay
Argument(s) : delay_ms - Time to delay in ms
Return value : None.
--------------------------------------------------------------------------------------------------*/
void hw_delay(uint16_t delay_ms)
{
	int i;
	//put delay into output compare register
	OCR2A = 15;
	//clear output compare flag
	TIFR2 |= _BV(OCF2A);
	//reset timer
	TCNT2 = 0;
	
	for(i = 0; i < delay_ms; i++)
	{
		//wait for compare flag to be high
		while(!(TIFR2 & _BV(OCF2A)));
		//clear compare flag
		TIFR2 |= _BV(OCF2A);
	}
}

/*************************************************
Function that enables and disables the PWM that is connected
to the servo circuit
*************************************************/
/*--------------------------------------------------------------------------------------------------
Name : Timer1PWM_OnOff
Description : Function that enables and disables the Timer 1 PWM to turn on and off the bell alarm
Argument(s) : state - 1 turns PWM on and 0 turns PWM off
Return value : None.
--------------------------------------------------------------------------------------------------*/
void Timer1PWM_OnOff(uint8_t state){
	if (state){
		TCCR1B |= _BV(CS12) | _BV(CS10);
		TCCR1A = _BV(COM1A1);
		
	}
	else{
		TCCR1B &= ~_BV(CS12) & ~_BV(CS10);
		PORTB &= ~_BV(PB1);	
		TCCR1A = 0;
	}
}