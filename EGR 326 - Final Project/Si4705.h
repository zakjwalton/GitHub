/*
 * Si4705.h
 *
 * Created: 11/1/2014 5:38:36 PM
 *  Author: Zak Walton and John LaFavor
 */ 

#include	<stdio.h>
#include	<avr/io.h>
#include	"I2C.h"

#ifndef SI4705_H_
#define SI4705_H_

//Initialize 4705
void FM_Init(void);
//Tell the 4705 to seek up or down
void FM_Seek(uint8_t direction);
//Tell the 4705 to tune to a specific station
void FM_Tune(uint16_t station);
//Get the current station
uint16_t FM_GetStation(void);
//Get the stereo signal strength as well as stereo or mono mode
uint16_t FM_Stereo(void);
//Set a volume level 0-63
void FM_Volume(uint8_t volume);





#endif /* SI4705_H_ */