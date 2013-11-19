/*
 * display.h
 *
 * Created: 02.10.2013 23:45:00
 *  Author: mortenmj
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

typedef enum
{
	DISPLAY_MENU,
	DISPLAY_GAME,
	N_DISPLAY_MODES
} display_mode_t;

typedef struct display_struct
{
	display_mode_t mode;
	uint8_t alive;
};

void vDisplay ( void *pvParameters );

#endif /* DISPLAY_H_ */