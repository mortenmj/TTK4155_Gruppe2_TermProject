/*
 * display.c
 *
 * Created: 02.10.2013 23:45:09
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "task.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "u8glib/u8g.h"
#include "display.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 20 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

u8g_t u8g;

static unsigned char ucGetButtons ( void )
{
	/* Mask out LEDs from the port */
	return SWPIN & ((1 << SW0) | (1 << SW1) | (1 << SW2) | (1 << SW3));
}

void vDisplay ( void *pvParameters )
{
	uint8_t button;
	char cbutton[4];
	
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();
	
	buttons_init ();
	menu_init ();
	
	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );
	
	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, dispTASK_FREQUENCY );
		
		if (buttons_get (&button, 0))
		{
			menu_update (button);
		}
		
		u8g_FirstPage(&u8g);
		do
		{
			menu_display (&u8g);
		} while ( u8g_NextPage( &u8g ) );
	}
}