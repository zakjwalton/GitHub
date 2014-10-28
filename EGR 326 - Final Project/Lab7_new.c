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
#include "FSM.h"

#define		PWM_ON	0x01
#define		PWM_OFF	0x00
#define		BUTTON_ON	DDRD &= ~_BV(PD6)

void RTC_Read_Sec(uint8_t *second)
{
	*second = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, SECONDS_R));
}

/*************************************************
Function that adds 30 seconds to the current time to act as
an alarm
*************************************************/

uint8_t Update_alarm(uint8_t *seconds)
{
	uint8_t alarm_time;
	RTC_Read_Sec(seconds);
	alarm_time = *seconds + 30;
	if(alarm_time > 60)
	{
		alarm_time = alarm_time - 60;
	}
	return(alarm_time);
}

/*************************************************
Function that implements a 15 second timeout by adding
15 seconds to the current time
*************************************************/

uint8_t Update_timeout(uint8_t *seconds)
{
	uint8_t timeout;
	RTC_Read_Sec(seconds);
	timeout = *seconds + 15;
	if(timeout > 60)
	{
		timeout = timeout - 60;
	}
	return(timeout);
}


int main(void)
{
	//initialize I2C, timers and button
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	I2C_Init();
	BUTTON_init();
	
	//initialize variables
	uint8_t alarm_time;
	uint8_t seconds;
	uint16_t delay_time = 5;
	uint8_t timeout;
	
	
	sei();
	//main loop

	while(1)
	{
		
	}
