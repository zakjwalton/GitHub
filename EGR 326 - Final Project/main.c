/*
 * main.c
 *
 * Created: 10/15/2014 3:16:30 PM
 *  Author: Zak Walton and Jon LaFavor
 *  EGR 326 Final Project - Alarm Clock Radio
 */ 

/*
	Still need to add functionality for radio as well
as switches for alarms, switches should be easy just haven't
bought any switches yet.  Also need to add ADC stuff for 
Thermistor and Photo sensor.  Need to add PWM functionality
for LCD back light, should be able to pull from previous lab as
well as Photo sensor stuff.  Probably should use ADC in some sort
of interrupt mode??  Need to be able to read the temp and then the
photocell and switch back and forth.  Maybe toggle sort of variable
in the interrupt?  Planning to control volume using an analog pot
but could still add volume crescendo for alarm if I have time.

*/

//Standard includes
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <avr/wdt.h>

//Custom libraries
//#include "I2C.h"
#include "i2cmaster.h"
#include "Buttons.h"
#include "RTC.h"
#include "LCD_PCD8544.h"
#include "Timers.h"



//Useful Macros
#define		CLEAR_BUTTONS (g_button_pressed = 0)
#define		PWM_ON	0x01
#define		PWM_OFF	0x00
#define		WDT_RESET WDT_counter = 0
#define		WDT_TIMEOUT	15

/*Global Variables*/
//Variable for storing the button pressed/held
BYTE g_button_pressed = 0;

//Structures for alarms and current time
alarm_t alarmtime1;
alarm_t alarmtime2;
time_t timeset; // Only used for time setting
time_t times;

//Temporary string for storing state of alarms
//  to be replaced by cute pictures
char string_alarm_onoff1[10];
char string_alarm_onoff2[10];
const char on[] = "ON ";
const char off[] = "OFF";

//Variables to make sure alarm doesn't continually trigger while
//  the minute hasn't changed
uint8_t alarmfresh1 = 0;
uint8_t alarmfresh2 = 0;

//variable used to exit menu when back is selected
uint8_t menufresh = 0;

//variable used to store the current menu selection
int menu_current = 0;

//Variable for storing the WDT count, i.e. WDT_counter = 2, time 8s
uint8_t WDT_counter = 0;

//EEPROM variables for alarms
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
inline void check_alarms(void);
void WDT_Init(void);

/*--------------------------------------------------------------------------------------------------
Name : main
Description : Call initialization functions and implement program state machine
Argument(s) : None.
Return value : int (0)
--------------------------------------------------------------------------------------------------*/

int main(void)
{
	//initialize peripherals, timers and buttons/switches
	cli();
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	WDT_Init();
	//I2C_Init();
	i2c_init();
	BUTTON_init();
	LCD_SPI_initialize();
	LCD_initialize();
	//FM_Init(); Yet to be implemented
	//ADC_Init(); Yet to be implemented
	sei();
	
	/* Set time if needed
	timeset.hour = 9;
	timeset.minute = 36;
	timeset.second = 0;
	timeset.AM_PM = 1;
	timeset.day = 29;
	timeset.day_of_week = 6;
	timeset.month = 10;
	timeset.year = 14;

	RTC_Set(currenttime);
	*/
	
	
	// Read alarm time from EEPROM
	eeprom_GetAlarm();
	//Clear out random stuff when re-programming
	alarmtime1.on_off &= 0x01;
	alarmtime2.on_off &= 0x01;
	alarmtime1.alarm_number = 1;
	alarmtime2.alarm_number = 2;
	//alarmtime1.music_bell;
	
	//Set alarm string to proper value, should be replaced by cute symbol
	if(alarmtime1.on_off){
		strcpy(string_alarm_onoff1,on);
	}
	else
	{
		strcpy(string_alarm_onoff1,off);
	}
	if(alarmtime2.on_off){
		strcpy(string_alarm_onoff2,on);
	}
	else
	{
		strcpy(string_alarm_onoff2,off);
	}
	
	while(1)
	{
		//Main switch for turning on radio, toggling presets, entering
		// the menu, and toggling alarms on and off
		switch (g_button_pressed){
			//button one pressed
			case (B1):
				CLEAR_BUTTONS;
				//toggle presets
				break;
			//button 2 pressed, enter menu
			case (B2):
				CLEAR_BUTTONS;
				//Set menu loop variable
				menufresh = 1;
				//Set the select to first element in the menu
				menu_current = 0;
				LCD_clear_screen();
				//Loop for main menu while the WDT hasn't timed out and 
				// the user hasn't selected back
				while(menufresh && (WDT_counter < WDT_TIMEOUT))
				{
					//print menu with current selection inverted
					LCD_print_menu(menu_current);
					switch(g_button_pressed){
						//move one selection up in menu
						case (B1):
							CLEAR_BUTTONS;
							menu_current = menu_current-1;
							break;
						//enter state for current menu selection
						case (B2):
							CLEAR_BUTTONS;
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
									break;
								case(4):
									//set Alarm2
									set_alarm(&alarmtime2);
									break;
								case(5):
									//back
									menufresh = 0;
									LCD_clear_screen();
									break;
							}
							break;
						//move one selection down in menu
						case (B3):
							CLEAR_BUTTONS;
							//WDT_RESET;
							menu_current++;
							break;
					}
					//Handle going over the top or below the bottom selection
					if(menu_current == 6){
						menu_current = 0;
					}
					else if(menu_current == -1){
						menu_current = 5;
					}
				}
				break;
			//toggle alarm 1 on or off
			case (B3):
				CLEAR_BUTTONS;
				//toggle alarm 1
				alarmtime1.on_off ^= 0x01;
				eeprom_update_byte(&alarm_onoff1, alarmtime1.on_off);
				//update string to be displayed
				if(alarmtime1.on_off){
					strcpy(string_alarm_onoff1,on);
				}
				else{
					strcpy(string_alarm_onoff1,off);
				}
				break;
			case(B1H):
				CLEAR_BUTTONS;
				//radio on/off
				break;
			case(B2H):
				CLEAR_BUTTONS;
				break;
			//toggle alarm 2 on or off (to be replaced with switches)
			case(B3H):
				CLEAR_BUTTONS;
				//toggle alarm 2
				alarmtime2.on_off ^= 0x01;
				eeprom_update_byte(&alarm_onoff2, alarmtime2.on_off);
				//update string to be displayed
				if(alarmtime2.on_off){
					strcpy(string_alarm_onoff2,on);
				}
				else{
					strcpy(string_alarm_onoff2,off);
				}
				break;
			//default alarm and radio display
			//need to add if statement that tailors display for the radio being on/off
			default:
				//If the counter is greater than timeout, reset it
				if(WDT_counter >= WDT_TIMEOUT)
				{
					WDT_RESET;
					//Clear screen in case UI timed out
					LCD_clear_screen();
				}
				
				//Read current time and update main display
				RTC_Read(&times);
				LCD_print_time_display(times, 21,"RADIO", "MENU", string_alarm_onoff1, string_alarm_onoff2);
				
				//check if alarms have gone off, and if so turn alarm on
				check_alarms();
				break;
		}
		
		
	}
	return (0);
}

/*--------------------------------------------------------------------------------------------------
Name : check_alarms
Description : Function that checks to see if alarm has gone off and if so, turns on alarm until the user
silences or turns off, or 15 seconds is up
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

inline void check_alarms(void){
	//check if alarm 1 has gone off
	if((alarmtime1.hour == times.hour) && (alarmtime1.minute == times.minute) && (alarmtime1.AM_PM == times.AM_PM)
	&& (alarmtime1.on_off) && (alarmfresh1 != times.minute))
	{
		//update variable to make sure alarm doesn't repeatedly go off for a minute
		alarmfresh1 = times.minute;
		Timer1PWM_OnOff(PWM_ON);
		LCD_clear_screen();
		LCD_goto(0,0);
		LCD_print_string("ALARM 1!",0);
		LCD_print_bottom_menu("OFF", " ", " ");
		CLEAR_BUTTONS;
		WDT_RESET;
		while(!g_button_pressed && (WDT_counter < (WDT_TIMEOUT/4))){
			RTC_Read(&times);
			hw_delay(5);
		}
		Timer1PWM_OnOff(PWM_OFF);
		CLEAR_BUTTONS;
		//WDT_RESET;
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
		//WDT_RESET;
		while(!g_button_pressed && (WDT_counter < (WDT_TIMEOUT/4))){
			RTC_Read(&times);
			hw_delay(5);
		}
		Timer1PWM_OnOff(PWM_OFF);
		CLEAR_BUTTONS;
		//WDT_RESET;
	}

}

/*--------------------------------------------------------------------------------------------------
Name : set_time
Description : Function that walks the user through setting the time
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

inline void set_time(void)
{
	
}

/*--------------------------------------------------------------------------------------------------
Name : set_alarm
Description : Initializing ATmega328P LCD for using with Nokia 5110 LCD w/ PCD8544 Controller
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

inline void set_alarm(alarm_t *alarmtime){
	LCD_print_alarm_display(*alarmtime,HOURS);
	CLEAR_BUTTONS;
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		switch(g_button_pressed){
			case(B1):
			//WDT_RESET;
			if(alarmtime->hour <= 0){
				alarmtime->hour = 13;
				break;
			}
			alarmtime->hour = alarmtime->hour - 1;
			LCD_print_alarm_display(*alarmtime,HOURS);
			CLEAR_BUTTONS;
			break;
			case(B3):
			//WDT_RESET;
			alarmtime->hour++;
			if(alarmtime->hour >= 13){
				alarmtime->hour = 0;
			}
			LCD_print_alarm_display(*alarmtime,HOURS);
			CLEAR_BUTTONS;
			break;
		}
		hw_delay(5);
	}
	CLEAR_BUTTONS;
	LCD_print_alarm_display(*alarmtime,MINUTES);
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		switch(g_button_pressed){
			case(B1):
			//WDT_RESET;
			if(alarmtime->minute <= 0){
				alarmtime->minute = 60;
				break;
			}
			alarmtime->minute = alarmtime->minute - 1;
			LCD_print_alarm_display(*alarmtime,MINUTES);
			CLEAR_BUTTONS;
			break;
			case(B3):
			//WDT_RESET;
			alarmtime->minute++;
			if(alarmtime->minute >= 60){
				alarmtime->minute = 0;
			}
			LCD_print_alarm_display(*alarmtime,MINUTES);
			CLEAR_BUTTONS;
			break;
		}
		hw_delay(5);
		// Update alarm set time in EEPROM
	}
	CLEAR_BUTTONS;
	LCD_print_alarm_display(*alarmtime,AMPM);
	while((g_button_pressed != B2) && (WDT_counter < WDT_TIMEOUT))
	{
		hw_delay(5);
		if((g_button_pressed == B1) || (g_button_pressed == B3) )
		{
			//WDT_RESET;
			alarmtime->AM_PM++;
			if(alarmtime->AM_PM >= 2){
				alarmtime->AM_PM = 0;
			}
			LCD_print_alarm_display(*alarmtime,AMPM);
			CLEAR_BUTTONS;
		}
		// Update alarm set time in EEPROM
	}
	//store new alarm time in EEPROM
	eeprom_SetAlarm();
	CLEAR_BUTTONS;
	LCD_clear_screen();
}

/*--------------------------------------------------------------------------------------------------
Name : eeprom_GetAlarm
Description : Fetches the current alarm value from EEPROM on startup and stores it into the alarm structs
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void eeprom_GetAlarm(void){
	// Read alarm time from EEPROM
	alarmtime1.hour = (uint8_t)eeprom_read_byte(&alarm_hour1);
	alarmtime1.minute = (uint8_t)eeprom_read_byte(&alarm_minute1);
	alarmtime1.AM_PM = (uint8_t)eeprom_read_byte(&alarm_AM_PM1);
	alarmtime1.on_off = (uint8_t)eeprom_read_byte(&alarm_onoff1);
	alarmtime2.hour = (uint8_t)eeprom_read_byte(&alarm_hour2);
	alarmtime2.minute = (uint8_t)eeprom_read_byte(&alarm_minute2);
	alarmtime2.AM_PM = (uint8_t)eeprom_read_byte(&alarm_AM_PM2);
	alarmtime2.on_off = (uint8_t)eeprom_read_byte(&alarm_onoff2);
}

/*--------------------------------------------------------------------------------------------------
Name : eeprom_SetAlarm
Description : Stores the values currently in the alarm structs into EEPROM, called when alarm is set
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void eeprom_SetAlarm(void){
	//store alarm time
	eeprom_update_byte(&alarm_hour1, alarmtime1.hour);
	eeprom_update_byte(&alarm_minute1, alarmtime1.minute);
	eeprom_update_byte(&alarm_AM_PM1, alarmtime1.AM_PM);
	eeprom_update_byte(&alarm_hour2, alarmtime2.hour);
	eeprom_update_byte(&alarm_minute2, alarmtime2.minute);
	eeprom_update_byte(&alarm_AM_PM2, alarmtime2.AM_PM);
	
	
}

/*--------------------------------------------------------------------------------------------------
Name : WDT_Init
Description : Initializes the WDT in interrupt mode prescaled down to 4s
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void WDT_Init(void)
{
	
	//reset watchdog
	wdt_disable();
	//set up WDT interrupt
	WDTCSR = (1<<WDCE)|(1<<WDE);
	//Start watchdog timer with 4s prescaller
	WDTCSR = (1<<WDIE)|(1<<WDP3);
}

/*--------------------------------------------------------------------------------------------------
Name : ADC_Init
Description : Initialization function for ADC for thermistor and Photo cell
Argument(s) : Possibly pass in a variable to tell what conversion to do??
Return value : None.
--------------------------------------------------------------------------------------------------*/

void ADC_Init(void)
{
	//Initialize ADC for two different conversions
}

/*--------------------------------------------------------------------------------------------------
Name : TIMER0_COMPA_vect
Description : ISR @ 1kHz for button reading/debouncing
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/
//Have yet to add switch handling and LEDs based on radio
ISR(TIMER0_COMPA_vect)
{
	static int count;
	static int bpressed = 0;
	int button1Raw = (BUTTON_READ_REG & _BV(BUTTON_1)) == 0;
	int button2Raw = (BUTTON_READ_REG & _BV(BUTTON_2)) == 0;
	int button3Raw = (BUTTON_READ_REG & _BV(BUTTON_3)) == 0;
	
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
		WDT_RESET;
	}
	
	if(bpressed && !button1Raw && !button2Raw && !button3Raw)
	{
		g_button_pressed = bpressed;
		bpressed = 0;
		count = 0;
		WDT_RESET;
	}
	
}

/*--------------------------------------------------------------------------------------------------
Name : WDT_vect
Description : Interrupt every 4 seconds from WDT used for UI timeouts and system reset
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

ISR(WDT_vect)
{
	WDT_counter++;
	
	//Still need to add functionality for System reset
}

/*--------------------------------------------------------------------------------------------------
Name : ADC_vect
Description : Interrupt used for ADC for thermistor as well as Photo sensor
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

ISR(ADC_vect)
{
	//add logic for handling 2 ADCs
}
