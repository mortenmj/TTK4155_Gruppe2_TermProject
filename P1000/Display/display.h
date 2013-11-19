/*
 * display.h
 *
 * Created: 02.10.2013 23:45:00
 *  Author: mortenmj
 */


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "u8glib/u8g.h"

void vDisplay ( void *pvParameters );
xTaskHandle xDisplayGetHandle (void);
void vDisplaySetHandle (xTaskHandle handle);
void vDisplaySetDisplayFunc (void (*func)(u8g_t *u8g));

#endif /* DISPLAY_H_ */