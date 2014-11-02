/*
 * Si4705.c
 *
 * Created: 11/1/2014 5:37:07 PM
 *  Author: Zak Walton and John LaFavor
 */ 

#include "Si4705.h"

/*--------------------------------------------------------------------------------------------------
Name : FM_Init
Description : Initializes the Si4705 FM chip by setting right settings and properties
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/
void FM_Init(void)
{
	
}

/*--------------------------------------------------------------------------------------------------
Name : FM_Seek
Description : Tells the FM chip to seek in the specified direction
Argument(s) : direction - direction to seek
Return value : None.
--------------------------------------------------------------------------------------------------*/
void FM_Seek(uint8_t direction)
{
	
}

/*--------------------------------------------------------------------------------------------------
Name : FM_Tune
Description : Tunes the FM chip to the specified station
Argument(s) : station - Station to tune to
Return value : None.
--------------------------------------------------------------------------------------------------*/
void FM_Tune(uint16_t station)
{
	
}

/*--------------------------------------------------------------------------------------------------
Name : FM_GetStation
Description : Read the current station from the FM chip
Argument(s) : None.
Return value : 16 bit station value
--------------------------------------------------------------------------------------------------*/
uint16_t FM_GetStation(void)
{
	return 0;
}

/*--------------------------------------------------------------------------------------------------
Name : FM_Stereo
Description : Get the stereo signal strength
Argument(s) : None.
Return value : Stereo signal strength
--------------------------------------------------------------------------------------------------*/
uint16_t FM_Stereo(void)
{
	return 0;
}

/*--------------------------------------------------------------------------------------------------
Name : FM_Volume
Description : Set a volume level 0-63
Argument(s) : 8 bit volume to set (0-63)
Return value : None.
--------------------------------------------------------------------------------------------------*/
void FM_Volume(uint8_t volume)
{
	
}