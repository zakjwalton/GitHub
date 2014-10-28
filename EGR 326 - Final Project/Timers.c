/*
 * Timers.c
 *
 * Created: 10/13/2014 1:23:56 PM
 *  Author: Jonathan
 */ 

#include <avr/io.h>

/*************************************************
Function that setup up Timer 0 to trigger an interrupt every 
*************************************************/

void Timer0_Init(void){
	//Setup timer 0 w/ interrupt every ms and pre-scale of 64
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(OCIE0A);
	OCR0A = 250;
	TIFR0 |= _BV(OCF0A);
	TCNT0 = 0;
}

/*************************************************
Function that setup up Timer 2 in CTC mode with 1024 prescalar
*************************************************/

void Timer2_Init(void){
	//Setup timer 1 to run in CTC mode 4 with 1024 prescalar
	TCCR2A = _BV(WGM21);
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
	
}

/*************************************************
Function that initializes timer1 in PWM mode @20Hz with
a 25% duty cycle
*************************************************/

void Timer1_Init()
{
	DDRB |= _BV(PB1);
	TCCR1A = _BV(COM1A1);
	TCCR1B = _BV(WGM13);
	ICR1 = 390;
	OCR1A = 90;
}

/*************************************************
Function that takes in a delay in ms and uses Timer 2 to
implement the delay in ms
*************************************************/

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