/*
 * joystick.c
 *
 * Created: 29.09.2013 29:09:36
 *  Author: magnealv
 */ 

#include "joystick.h"
#include <stdlib.h>

uint8_t getpercentage(uint8_t value)
{
	return ((value*200/ADC_MAX_VAL)-100);
}


direction_t getdirection(uint8_t *values)
{
	uint8_t x_perc = getpercentage(values[X]);
	uint8_t y_perc = getpercentage(values[Y]);
	
	if (abs(x_perc) > abs(y_perc))
	{
		if(x_perc < 0-JOYSTICK_SLACK){ return LEFT; }
		if(x_perc > 0+JOYSTICK_SLACK){ return RIGHT; }
		else{ return CENTER; }		
	}
	if (abs(x_perc) < abs(y_perc))
	{
		if(y_perc < 0-JOYSTICK_SLACK){ return DOWN; }
		if(y_perc > 0+JOYSTICK_SLACK){ return UP; }
		else{ return CENTER; }
	}
	else
	{
		return CENTER;
	}
}

