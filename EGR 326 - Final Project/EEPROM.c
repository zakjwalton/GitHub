/*
 * EEPROM.c
 *
 * Created: 10/24/2014 11:51:57 AM
 *  Author: Zachary
 */ 

#include "EEPROM.h"


void eeprom_GetAlarm(void){
	// Read alarm time from EEPROM
	alarmtime.hour = (uint8_t)eeprom_read_byte(&alarm_hour);
	alarmtime.minute = (uint8_t)eeprom_read_byte(&alarm_minute);
}

void eeprom_SetAlarm(void){
	//store alarm time
	eeprom_update_byte(&alarm_hour, alarmtime.hour);
	eeprom_update_byte(&alarm_minute, alarmtime.minute);
}