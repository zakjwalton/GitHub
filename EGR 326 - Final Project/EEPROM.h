/*
 * EEPROM.h
 *
 * Created: 10/24/2014 11:53:25 AM
 *  Author: Zachary
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

// EEPROM variables
extern uint8_t EEMEM alarm_hour;
extern uint8_t EEMEM alarm_minute;

void eeprom_GetAlarm(void);
void eeprom_SetAlarm(void);




#endif /* INCFILE1_H_ */