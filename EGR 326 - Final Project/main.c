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
#include <avr/eeprom.h>
#include <stdbool.h>

#include "Buttons.h"
#include "LCD_PCD8544.h"
#include "Timers.h"
#include "I2C.h"
#include "RTC.h"

#define		CLEAR_BUTTONS (g_button_pressed = 0)
#define		PWM_ON	0x01
#define		PWM_OFF	0x00

//global variables
BYTE g_button_pressed = 0;
uint8_t g_alarmOnOff = 0;
time_t alarmtime;
time_t currenttime;
time_t times;

uint8_t EEMEM alarm_hour;
uint8_t EEMEM alarm_minute;
uint8_t EEMEM alarm_AM_PM;

int Update_timeout(void)
{
	int timeout;
	timeout = times.second + 15;
	if(timeout > 60)
	{
		timeout = timeout - 60;
	}
	return(timeout);
}

ISR(TIMER0_COMPA_vect)
{
	static int count;
	static int bpressed = 0;
	bool button1Raw = (PINC & _BV(BUTTON_1)) == 0;
	bool button2Raw = (PINC & _BV(BUTTON_2)) == 0;
	bool button3Raw = (PINC & _BV(BUTTON_3)) == 0;
	
	if(button1Raw){
		count++;
		if(count == 50){
			bpressed = B1;
		}
		if(count == 700){
			bpressed = B1H;
		}
	}
	
	if(button2Raw){
		count++;
		if(count == 50){
			bpressed = B2;
		}
		if(count == 700){
			bpressed = B2H;
		}
	}
	
	if(button3Raw){
		count++;
		if(count == 50){
			bpressed = B3;
		}
		if(count == 700){
			bpressed = B3H;
		}
	}
	if((bpressed > B3) && !button1Raw && !button2Raw && !button3Raw){
		g_button_pressed = bpressed;
		bpressed = 0;
		count = 0;
	}
	
	if(bpressed && !button1Raw && !button2Raw && !button3Raw)
	{
		g_button_pressed = bpressed;
		bpressed = 0;
		count = 0;
	}
	
}

void eeprom_GetAlarm(void){
	// Read alarm time from EEPROM
	alarmtime.hour = (uint8_t)eeprom_read_byte(&alarm_hour);
	alarmtime.minute = (uint8_t)eeprom_read_byte(&alarm_minute);
	alarmtime.AM_PM = (uint8_t)eeprom_read_byte(&alarm_AM_PM);
}

void eeprom_SetAlarm(void){
	//store alarm time
	eeprom_update_byte(&alarm_hour, alarmtime.hour);
	eeprom_update_byte(&alarm_minute, alarmtime.minute);
	eeprom_update_byte(&alarm_AM_PM, alarmtime.AM_PM);
}


int main(void)
{
	cli();
	//initialize I2C, timers and button
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	I2C_Init();
	BUTTON_init();
	LCD_SPI_initialize();
	LCD_initialize();
	sei();
	
	
	char alarm_onoff[10];
	char on[] = "ON ";
	char off[] = "OFF";
	int timeout;
	uint8_t fresh = 0;
	
	
	strcpy(alarm_onoff,off);
	
	/*
	currenttime.hour = 9;
	currenttime.minute = 36;
	currenttime.second = 0;
	currenttime.AM_PM = 1;
	currenttime.day = 29;
	currenttime.day_of_week = 6;
	currenttime.month = 10;
	currenttime.year = 14;

	RTC_Set(currenttime);
	*/
	
	
	// Read alarm time from EEPROM
	eeprom_GetAlarm();
	while(1)
	{
		
		RTC_Read(&times);
		LCD_print_time_display(times, 21,"SET", "ALARM", alarm_onoff);
		
		if((alarmtime.hour == times.hour) && (alarmtime.minute == times.minute) && (alarmtime.AM_PM == times.AM_PM)
											 && (g_alarmOnOff) && (fresh != times.minute))
		{
			fresh = times.minute;
			Timer1PWM_OnOff(PWM_ON);
			LCD_clear_screen();
			LCD_goto(0,0);
			LCD_print_string("ALARM!");
			LCD_print_bottom_menu("OFF", " ", " ");
			CLEAR_BUTTONS;
			timeout = Update_timeout();
			while(!g_button_pressed && (times.second != timeout)){
				RTC_Read(&times);
				hw_delay(5);
			}
			Timer1PWM_OnOff(PWM_OFF);
			CLEAR_BUTTONS;
		}

		switch (g_button_pressed){
			case (B1):
				Timer1PWM_OnOff(PWM_OFF);
				CLEAR_BUTTONS;
				break;
			case (B2):
				LCD_print_alarm_display(alarmtime,"UP","ENTER"," ");
				CLEAR_BUTTONS;
				while(g_button_pressed != B2)
				{
					hw_delay(5);
					if(g_button_pressed == B1)
					{
						alarmtime.hour++;
						if(alarmtime.hour == 13){
							alarmtime.hour = 0;
						}
						LCD_print_alarm_display(alarmtime,"UP","ENTER"," ");
						CLEAR_BUTTONS;
					}
				}
				CLEAR_BUTTONS;
				while(g_button_pressed != B2)
				{
					hw_delay(5);
					if(g_button_pressed == B1)
					{
						alarmtime.minute++;
						if(alarmtime.minute == 60){
							alarmtime.minute = 0;
						}
						LCD_print_alarm_display(alarmtime,"UP","ENTER"," ");
						CLEAR_BUTTONS;
					}
					// Update alarm set time in EEPROM
				}
				CLEAR_BUTTONS;
				while(g_button_pressed != B2)
				{
					hw_delay(5);
					if(g_button_pressed == B1)
					{
						alarmtime.AM_PM++;
						if(alarmtime.AM_PM >= 2){
							alarmtime.AM_PM = 0;
						}
						LCD_print_alarm_display(alarmtime,"UP","ENTER"," ");
						CLEAR_BUTTONS;
					}
					// Update alarm set time in EEPROM
				}
				//store new alarm time in EEPROM
				eeprom_SetAlarm();
				CLEAR_BUTTONS;
				break;
			case (B3):
				g_alarmOnOff ^= 0x01;
				if(g_alarmOnOff){
					strcpy(alarm_onoff,on);
				}
				else{
					strcpy(alarm_onoff,off);
				}
				CLEAR_BUTTONS;
				break;
			case(B1H):
				break;
			case(B2H):
				break;
			case(B3H):
				
				break;
		}
		
		
	}
}