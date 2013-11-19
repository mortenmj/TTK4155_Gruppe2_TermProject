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

#include "display.h"

#include "fsm.h"
#include "u8glib/u8g.h"
#include "menu.h"
#include "score.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 20 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

u8g_t u8g;
xTaskHandle task_handle;
void (*display_handler)(u8g_t*);

xTaskHandle xDisplayGetHandle (void)
{
    return task_handle;
}

void vDisplaySetHandle (xTaskHandle handle)
{
    task_handle = handle;
}

void vDisplaySetDisplayFunc (void (*func)(u8g_t *u8g))
{
	display_handler = func;
}

void vDisplay ( void *pvParameters )
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();

	menu_init ();
	score_init ();

	/* Set initial display handler */
	vDisplaySetDisplayFunc (&menu_display);

	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );

	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);

	while (1)
	{
		vTaskDelayUntil (&xLastWakeTime, dispTASK_FREQUENCY);

        menu_update ();

		u8g_FirstPage(&u8g);
		do
		{
			(*display_handler) (&u8g);
		} while ( u8g_NextPage( &u8g ) );
	}
}