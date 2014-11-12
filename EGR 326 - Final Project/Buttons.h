/*
 * Buttons.h
 *
 * Created: 10/11/2014 3:53:58 PM
 *  Author: Jonathan Lafavor and Zak Walton
 */ 

#include <avr/io.h>
#include <stdio.h>

#ifndef BUTTONS_H_
#define BUTTONS_H_

//Note: Still need to add switches and LEDs

//Defining button numbers
#define B1  1
#define B2  2
#define B3  3
#define B1H 48
#define B2H 5
#define B3H 6
#define BUTTON_1 PD0
#define BUTTON_2 PD1
#define BUTTON_3 PD5
#define BUTTON_READ_REG PIND
#define	BUTTONS_PORT PORTD

//Define switch numbers
#define S1 1
#define S2 2
#define SWITCH_1 PD6
#define SWITCH_2 PD7

//Define LED pins
#define LED_RADIO PC3
#define LED_STEREO PC6
 

// LCD pin data direction registers
#define BUTTON_DD_1           DDRD  // Previous State Button B1
#define BUTTON_DD_2           DDRD  // Settings of Current Display B2
#define BUTTON_DD_3           DDRD  // Next State Button B3
#define SWITCH_DD_1			  DDRD
#define SWITCH_DD_2			  DDRD
#define LED_DD_RADIO	      DDRC
#define LED_DD_STEREO		  DDRC

//Define LED Setting Macros
#define LED_RADIO_ON PORTC &= ~_BV(LED_RADIO)
#define LED_RADIO_OFF	PORTC |= _BV(LED_RADIO)
#define LED_STEREO_ON PORTC &= ~_BV(LED_STEREO)
#define LED_STEREO_OFF	PORTC |= _BV(LED_STEREO)


//Initialize 3 Buttons, 2 Switches, and 2 LEDs
void BUTTON_init();




#endif /* BUTTONS_H_ */