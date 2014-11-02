/*
 * Buttons.h
 *
 * Created: 10/11/2014 3:53:58 PM
 *  Author: Jonathan
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
#define B1H 4
#define B2H 5
#define B3H 6
#define BUTTON_1 PC0
#define BUTTON_2 PC1
#define BUTTON_3 PC2

//Defining states for buttons
#define BACK_BUTTON       B1
#define SETTING_BUTTON    B2
#define FORWARD_BUTTON    B3

// LCD signals and pins
#define BUTTON_1_PIN          0   // Previous State Button B1      
#define BUTTON_2_PIN          1   // Settings of Current Display B2
#define BUTTON_3_PIN          2   // Next State Button B3


// LCD signals ports
#define BUTTON_1_PORT         PINC // Previous State Button B1
#define BUTTON_2_PORT         PINC // Settings of Current Display B2
#define BUTTON_3_PORT         PINC // Next State Button B3



// LCD pin data direction registers
#define BUTTON_DD_1           DDRC  // Previous State Button B1
#define BUTTON_DD_2           DDRC  // Settings of Current Display B2
#define BUTTON_DD_3           DDRC  // Next State Button B3

//Initialize 3 Buttons, 2 Switches, and 2 LEDs
void BUTTON_init();




#endif /* BUTTONS_H_ */