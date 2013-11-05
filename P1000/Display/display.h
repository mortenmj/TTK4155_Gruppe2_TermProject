/*
 * display.h
 *
 * Created: 02.10.2013 23:45:00
 *  Author: mortenmj
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define dispMAX_CHECKS 5

struct xInputState
{
	unsigned char ucState;
	unsigned char ucValues[dispMAX_CHECKS];
	unsigned char ucIndex;
};

void vDisplay ( void *pvParameters );


#endif /* DISPLAY_H_ */