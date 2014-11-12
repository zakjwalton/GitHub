/*
 * RTC.h
 *
 * Created: 9/26/2014 9:26:02 PM
 *  Author: Jonathan LaFavor and Zak Walton
 */ 

#include	<stdio.h>
#include	<avr/io.h>
//#include	"I2C.h"
#include "i2cmaster.h"

#ifndef		RTC_H_
#define		RTC_H_

//Structure for time keeping
#ifndef TIME_T
#define TIME_T
typedef struct time time_t;
struct time{
	//variables should be self-explanatory
	uint8_t year,
	month,
	day,
	day_of_week,
	hour,
	minute,
	second,
	AM_PM;  // AM = 0 PM = 1
};
#endif

//structure for alarms
#ifndef ALARM_T
#define ALARM_T
typedef struct alarm alarm_t;
struct alarm{
	uint8_t hour, //Alarm hour
	minute, //Alarm minute
	AM_PM, // AM = 0 PM = 1
	on_off, //Alarm on/off indicator (1 is on)
	alarm_number, //Alarm number (1 or 2)
	music_bell; //Whether alarm turns on Music or Bell
	
	uint16_t preset; // Preset station to turn on if alarm goes off
};
#endif //alarm_t

//Slave address
#define		RTC_ADDRESS	0b1101000
#define		RTC_WRITE ((RTC_ADDRESS<<1) & ~0x01)
#define		RTC_READ ((RTC_ADDRESS<<1) | 0x01)

//DS1307 Memory Addresses
#define		SECONDS_R	0x00
#define		MINUTES_R	0x01
#define		HOURS_R		0x02
#define		DAY_R		0x03
#define		DATE_R		0x04
#define		MONTH_R		0x05
#define		YEAR_R		0x06


//Function that converts a binary coded decimal number to a decimal
uint8_t fromBCD(uint8_t data_in);
//Function that converts a decimal number to a binary coded decimal
uint8_t toBCD(uint8_t data_out);
//Function that sets the RTC
void RTC_Set(time_t times);
//Function that sets the RTC
void RTC_Read(time_t *times);

#endif // RTC.h