/*
 * RTC.c
 *
 * Created: 9/26/2014 9:26:31 PM
 *  Author: Jonathan LaFavor and Zak Walton
 */ 

#include "RTC.h"

/*--------------------------------------------------------------------------------------------------
Name : fromBCD
Description : Converts incoming RTC data from binary coded decimal to integer
Argument(s) : data_in - Raw Data from RTC
Return value : Decoded integer
--------------------------------------------------------------------------------------------------*/

uint8_t fromBCD(uint8_t data_in){
	return ((data_in & 0x0F) + 10*((data_in>>4) & 0x07));
}

/*--------------------------------------------------------------------------------------------------
Name : toBCD
Description : Function that converts data to be sent to the RTC from an integer to a BCD
Argument(s) : data_out - Integer data to be sent out
Return value : BCD to send to RTC
--------------------------------------------------------------------------------------------------*/

uint8_t toBCD(uint8_t data_out){
	return (((data_out/10)<<4) | (data_out % 10));
}

/*--------------------------------------------------------------------------------------------------
Name : RTC_Set
Description : Function that takes in time structure and writes it to the RTC to set the time
Argument(s) : time_t - Data structure that represents the time to be set
Return value : None.
--------------------------------------------------------------------------------------------------*/

void RTC_Set(time_t times)
{
	i2c_start(RTC_WRITE);
	i2c_write(SECONDS_R);
	i2c_write(toBCD(times.second));
	i2c_write(toBCD(times.minute));
	if(times.AM_PM){ // Set to PM
		i2c_write(toBCD(times.hour) | _BV(5) | _BV(6));
	}
	else{ // Set to AM
		i2c_write((toBCD(times.hour) | _BV(6)) & ~_BV(5));
	}
	i2c_write(toBCD(times.day_of_week));
	i2c_write(toBCD(times.day));
	i2c_write(toBCD(times.month));
	i2c_write(toBCD(times.year));
	i2c_stop();
	
	/*
	I2C_SendMessage(RTC_ADDRESS, YEAR_R, toBCD(times.year));
	I2C_SendMessage(RTC_ADDRESS, MONTH_R, toBCD(times.month));
	I2C_SendMessage(RTC_ADDRESS, DATE_R, toBCD(times.day));
	I2C_SendMessage(RTC_ADDRESS, DAY_R, toBCD(times.day_of_week));
	if(times.AM_PM){ // Set to PM
		I2C_SendMessage(RTC_ADDRESS, HOURS_R, toBCD(times.hour) | _BV(5) | _BV(6));
	}
	else{ // Set to AM
		I2C_SendMessage(RTC_ADDRESS, HOURS_R, ((toBCD(times.hour) | _BV(6))) & ~_BV(5));
	}
	
	I2C_SendMessage(RTC_ADDRESS, MINUTES_R, toBCD(times.minute));
	I2C_SendMessage(RTC_ADDRESS, SECONDS_R, toBCD(times.second));
	*/
	
}

/*--------------------------------------------------------------------------------------------------
Name : RTC_Read
Description : Function that updates the current time from the RTC and stores it in the time_t data structure
Argument(s) : Pointer to the time_t data structure used for the current time
Return value : None.
--------------------------------------------------------------------------------------------------*/

void RTC_Read(time_t *times)
{
	//point to first address
	uint8_t hour_temp;
	//uint8_t err; // dummy return variable
	i2c_start(RTC_WRITE);
	i2c_write(SECONDS_R);
	//initialize read mode
	i2c_rep_start(RTC_READ);
	//read all the data
	times->second = fromBCD(i2c_readAck());
	times->minute = fromBCD(i2c_readAck());
	hour_temp = i2c_readAck();
	times->day_of_week = fromBCD(i2c_readAck());
	times->day = fromBCD(i2c_readAck());
	times->month = fromBCD(i2c_readAck());
	times->year = fromBCD(i2c_readNak());
	//split apart hour and am/pm
	times->hour  = fromBCD(hour_temp & 0x1F);
	times->AM_PM = (hour_temp & _BV(5))>>5;
	i2c_stop();
	
	
	/*
	times->year = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, YEAR_R));
	times->month = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, MONTH_R));
	times->day = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, DATE_R));
	times->day_of_week = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, DAY_R));
	times->hour  = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, HOURS_R) & 0x1F);
	times->minute = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, MINUTES_R));
	times->second = fromBCD(I2C_RecieveMessage(RTC_ADDRESS, SECONDS_R));
	times->AM_PM = (I2C_RecieveMessage(RTC_ADDRESS, HOURS_R) & _BV(5))>>5;
	*/
	
}