/*
 * main.c
 *
 * Created: 10/15/2014 3:16:30 PM
 *  Author: Zak Walton and Jon LaFavor
 *  EGR 326 Final Project - Alarm Clock Radio
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <avr/wdt.h>

#include "Buttons.h"
#include "LCD_PCD8544.h"
#include "Timers.h"
#include "I2C.h"
#include "RTC.h"

#define		CLEAR_BUTTONS (g_button_pressed = 0)
#define		PWM_ON	0x01
#define		PWM_OFF	0x00
#define		WDT_RESET WDT_counter = 0
#define		WDT_TIMEOUT	2

//global variables
BYTE g_button_pressed = 0;
uint8_t g_alarmOnOff1 = 0;
uint8_t g_alarmOnOff2 = 0;
alarm_t alarmtime1;
alarm_t alarmtime2;
time_t currenttime;
time_t times;
char string_alarm_onoff1[10];
char string_alarm_onoff2[10];
const char on[] = "ON ";
const char off[] = "OFF";
int timeout;
uint8_t alarmfresh1 = 0;
uint8_t alarmfresh2 = 0;
uint8_t menufresh = 0;
int menu_current = 0;
uint8_t WDT_counter = 0;

uint8_t EEMEM alarm_hour1;
uint8_t EEMEM alarm_minute1;
uint8_t EEMEM alarm_AM_PM1;
uint8_t EEMEM alarm_onoff1;
uint8_t EEMEM alarm_hour2;
uint8_t EEMEM alarm_minute2;
uint8_t EEMEM alarm_AM_PM2;
uint8_t EEMEM alarm_onoff2;

//function prototypes
inline void set_alarm(alarm_t *alarmtime);
void eeprom_GetAlarm(void);
void eeprom_SetAlarm(void);
inline int Update_timeout(void);
inline void check_alarms(void);
void WDT_Init(void);


int main(void)
{
	cli();
	//initialize I2C, timers and button
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	WDT_Init();
	I2C_Init();
	BUTTON_init();
	LCD_SPI_initialize();
	LCD_initialize();
	sei();
	
	/* Set time if needed
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
	//Set alarm string to proper value
	if(alarmtime1.on_off){
		strcpy(string_alarm_onoff1,on);
	}
	else
	{
		strcpy(string_alarm_onoff1,off);
	}
	if(alarmtime1.on_off){
		strcpy(string_alarm_onoff2,on);
	}
	else
	{
		strcpy(string_alarm_onoff2,off);
	}
	
	while(1)
	{
		if(WDT_counter >= WDT_TIMEOUT)
		{
			WDT_RESET;
			LCD_clear_screen();
		}
		RTC_Read(&times);
		LCD_print_time_display(times, 21,"RADIO", "MENU", string_alarm_onoff1, string_alarm_onoff2);
		
		//check if alarms have gone off
		check_alarms();

		switch (g_button_pressed){
			case (B1):
				WDT_RESET;
				CLEAR_BUTTONS;
				//toggle presets
				break;
			case (B2):
				WDT_RESET;
				//menu
				CLEAR_BUTTONS;
				menufresh = 1;
				menu_current = 0;
				LCD_clear_screen();
				while(menufresh && (WDT_counter < WDT_TIMEOUT))
				{
					LCD_print_menu(menu_current);
					switch(g_button_pressed){
						case (B1):
							CLEAR_BUTTONS;
							WDT_RESET;
							menu_current = menu_current-1;
							break;
						case (B2):
							CLEAR_BUTTONS;
							WDT_RESET;
							switch(menu_current)
							{
								case(0):
									//tune radio
									break;
								case(1):
									//set presets
									break;
								case(2):
									//set  Time
									break;
								case(3):
									//set Alarm 1
									set_alarm(&alarmtime1);
									CLEAR_BUTTONS;
									break;
								case(4):
									//set Alarm2
									set_alarm(&alarmtime2);
									CLEAR_BUTTONS;
									break;
								case(5):
									//back
									menufresh = 0;
									LCD_clear_screen();
									break;
							}
							break;
						case (B3):
							CLEAR_BUTTONS;
							WDT_RESET;
							menu_current++;
							break;
					}
					if(menu_current == 6){
						menu_current = 0;
					}
					else if(menu_current == -1){
						menu_current = 5;
					}
				}
				break;
			case (B3):
				WDT_RESET;
				//toggle alarm 1
				alarmtime1.on_off ^= 0x01;
				if(alarmtime1.on_off){
					strcpy(string_alarm_onoff1,on);
				}
				else{
					strcpy(string_alarm_onoff1,off);
				}
				CLEAR_BUTTONS;
				break;
			case(B1H):
				WDT_RESET;
				CLEAR_BUTTONS;
				//radio on/off
				break;
			case(B2H):
				WDT_RESET;
				CLEAR_BUTTONS;
				break;
			case(B3H):
				WDT_RESET;
				//toggle alarm 2
				alarmtime2.on_off ^= 0x01;
				if(alarmtime2.on_off){
					strcpy(string_alarm_onoff2,on);
				}
				else{
					strcpy(string_alarm_onoff2,off);
				}
				CLEAR_BUTTONS;
				break;
		}
		
		
	}
}

//functions
inline int Update_timeout(void)
{
	int timeout;
	timeout = times.second + 15;
	if(timeout > 60)
	{
		timeout = timeout - 60;
	}
	return(timeout);
}

inline void check_alarms(void){
	if((alarmtime1.hour == times.hour) && (alarmtime1.minute == times.minute) && (alarmtime1.AM_PM == times.AM_PM)
	&& (alarmtime1.on_off) && (alarmfresh1 != times.minute))
	{
		alarmfresh1 = times.minute;
		Timer1PWM_OnOff(PWM_ON);
		LCD_clear_screen();
		LCD_goto(0,0);
		LCD_print_string("ALARM 1!",0);
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
	
	if((alarmtime2.hour == times.hour) && (alarmtime2.minute == times.minute) && (alarmtime2.AM_PM == times.AM_PM)
	&& (alarmtime2.on_off) && (alarmfresh2 != times.minute))
	{
		alarmfresh2 = times.minute;
		Timer1PWM_OnOff(PWM_ON);
		LCD_clear_screen();
		LCD_goto(0,0);
		LCD_print_string("ALARM 2!",0);
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

}

inline void set_alarm(alarm_t *alarmtime){
	LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
	CLEAR_BUTTONS;
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		switch(g_button_pressed){
			case(B1):
			WDT_RESET;
			if(alarmtime->hour <= 0){
				alarmtime->hour = 13;
				break;
			}
			alarmtime->hour = alarmtime->hour - 1;
			LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
			CLEAR_BUTTONS;
			break;
			case(B3):
			WDT_RESET;
			alarmtime->hour++;
			if(alarmtime->hour >= 13){
				alarmtime->hour = 0;
			}
			LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
			CLEAR_BUTTONS;
			break;
		}
		hw_delay(5);
	}
	CLEAR_BUTTONS;
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		switch(g_button_pressed){
			case(B1):
			WDT_RESET;
			if(alarmtime->minute <= 0){
				alarmtime->minute = 60;
				break;
			}
			alarmtime->minute = alarmtime->minute - 1;
			LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
			CLEAR_BUTTONS;
			break;
			case(B3):
			WDT_RESET;
			alarmtime->minute++;
			if(alarmtime->minute >= 60){
				alarmtime->minute = 0;
			}
			LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
			CLEAR_BUTTONS;
			break;
		}
		hw_delay(5);
		// Update alarm set time in EEPROM
	}
	CLEAR_BUTTONS;
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		hw_delay(5);
		if((g_button_pressed == B1) || (g_button_pressed == B3) )
		{
			WDT_RESET;
			alarmtime->AM_PM++;
			if(alarmtime->AM_PM >= 2){
				alarmtime->AM_PM = 0;
			}
			LCD_print_alarm_display(*alarmtime,"UP","ENTER"," ");
			CLEAR_BUTTONS;
		}
		// Update alarm set time in EEPROM
	}
	//store new alarm time in EEPROM
	eeprom_SetAlarm();
	CLEAR_BUTTONS;
	LCD_clear_screen();
}

void eeprom_GetAlarm(void){
	// Read alarm time from EEPROM
	alarmtime1.hour = (uint8_t)eeprom_read_byte(&alarm_hour1);
	alarmtime1.minute = (uint8_t)eeprom_read_byte(&alarm_minute1);
	alarmtime1.AM_PM = (uint8_t)eeprom_read_byte(&alarm_AM_PM1);
	alarmtime2.hour = (uint8_t)eeprom_read_byte(&alarm_hour2);
	alarmtime2.minute = (uint8_t)eeprom_read_byte(&alarm_minute2);
	alarmtime2.AM_PM = (uint8_t)eeprom_read_byte(&alarm_AM_PM2);
}

void eeprom_SetAlarm(void){
	//store alarm time
	eeprom_update_byte(&alarm_hour1, alarmtime1.hour);
	eeprom_update_byte(&alarm_minute1, alarmtime1.minute);
	eeprom_update_byte(&alarm_AM_PM1, alarmtime1.AM_PM);
	eeprom_update_byte(&alarm_onoff1, alarmtime1.on_off);
	eeprom_update_byte(&alarm_hour2, alarmtime2.hour);
	eeprom_update_byte(&alarm_minute2, alarmtime2.minute);
	eeprom_update_byte(&alarm_AM_PM2, alarmtime2.AM_PM);
	eeprom_update_byte(&alarm_onoff2, alarmtime2.on_off);
	
	
}

void WDT_Init(void)
{
	
	//reset watchdog
	wdt_disable();
	//set up WDT interrupt
	WDTCSR = (1<<WDCE)|(1<<WDE);
	//Start watchdog timer with 4s prescaller
	WDTCSR = (1<<WDIE)|(1<<WDP3);
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

ISR(WDT_vect)
{
	WDT_counter++;
}
