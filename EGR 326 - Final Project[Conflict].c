/*
 * EGR_326___Final_Project.c
 *
 * Created: 10/15/2014 3:16:30 PM
 *  Author: zaks
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "Buttons.h"
#include "LCD_PCD8544.h"
#include "Timers.h"
#include "I2C.h"
#include "RTC.h"

int g_button_pressed;


ISR(PCINT1_vect)
{
	if(!(PINC && _BV(BUTTON_1))){
		g_button_pressed = B1;
	}
	else if(!(PINC && _BV(BUTTON_2))){
		g_button_pressed = B2;
	}
	else{
		g_button_pressed = B3;
	}
}


int main(void)
{
	cli();
	//initialize I2C, timers and button
	//Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	I2C_Init();
	BUTTON_init();
	LCD_SPI_initialize();
	LCD_initialize();
	sei();
	time_t time;
	time_t alarmtime;
	time.year = 1;
	time.AM_PM = 1;
	time.day_of_week = 1;
	time.day = 1;
	time.month = 1;
	time.hour = 1;
	time.minute = 1;
	time.second = 1;
	
	RTC_Set(time);
	
	while(1)
	{
	hw_delay(1000);
	LCD_print_time_display(time, 21, "ALARM", "SET", "RADIO");
	}
}