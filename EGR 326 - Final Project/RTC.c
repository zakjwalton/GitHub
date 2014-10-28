/*
 * RTC.c
 *
 * Created: 9/26/2014 9:26:31 PM
 *  Author: Jonathan LaFavor and Zak Walton
 */ 

#include "RTC.h"

/*************************************************
Function that converts a binary coded decimal number to a decimal
*************************************************/

uint8_t fromBCD(uint8_t data_in){
	return ((data_in & 0x0F) + 10*((data_in>>4) & 0x07));
}

/*************************************************
Function that converts a decimal number to a binary coded decimal
*************************************************/

uint8_t toBCD(uint8_t data_out){
	return (((data_out/10)<<4) | (data_out % 10));
}

/*************************************************
Function that sets the RTC
*************************************************/

void RTC_Set(time_t times)
{
	I2C_SendMessage(RTC_ADDRESS, YEAR_R, toBCD(times.year));
	I2C_SendMessage(RTC_ADDRESS, MONTH_R, toBCD(times.month));
	I2C_SendMessage(RTC_ADDRESS, DATE_R, toBCD(times.day));
	I2C_SendMessage(RTC_ADDRESS, DAY_R, toBCD(times.day_of_week));
	if(times.AM_PM){
		I2C_SendMessage(RTC_ADDRESS, HOURS_R, toBCD(times.hour) | _BV(5) | _BV(6));
	}
	else{
		I2C_SendMessage(RTC_ADDRESS, HOURS_R, ((toBCD(times.hour) | _BV(6))) & ~_BV(5));
	}
	
	I2C_SendMessage(RTC_ADDRESS, MINUTES_R, toBCD(times.minute));
	I2C_SendMessage(RTC_ADDRESS, SECONDS_R, toBCD(times.second));
	
}

/*************************************************
Function that reads the  from the RTC
*************************************************/

void RTC_Read(time_t *times)
{
	times->year = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, YEAR_R));
	times->month = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, MONTH_R));
	times->day = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, DATE_R));
	times->day_of_week = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, DAY_R));
	times->hour  = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, HOURS_R) & 0x1F);
	times->minute = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, MINUTES_R));
	times->second = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, SECONDS_R));
	times->AM_PM = (I2C_RecieveMessage(RTC_ADDRESS, HOURS_R) & _BV(5))>>5;
	
}