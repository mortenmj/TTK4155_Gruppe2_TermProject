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

#include "event.h"
#include "game.h"
#include "u8glib/u8g.h"
#include "menu.h"
#include "score.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 20 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

u8g_t u8g;

void vDisplay ( void *pvParameters )
{
	display_mode_t disp_mode = DISPLAY_MENU;
	event_t evnt;
	void (*display_func)(u8g_t*) = &score_display;
	
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();

	menu_init ();
	score_init ();

	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );

	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);

	/* Initial write */
	u8g_FirstPage(&u8g);
	do
	{
		menu_display (&u8g);
	} while ( u8g_NextPage( &u8g ) );

	while (1)
	{
		vTaskDelayUntil (&xLastWakeTime, dispTASK_FREQUENCY);
		
		/* Wait for event before updating display */
		if (event_get (&evnt, portMAX_DELAY))
		{
			switch (evnt.type)
			{
				case EVENT_BUTTON:
					menu_update (evnt.val);
					break;
				case EVENT_GAME:
					if (evnt.val == GAME_SENSOR_TRIGGERED)
					{
						vSerialPutString(NULL, "end\n", 4);
						score_reset ();
						disp_mode = DISPLAY_MENU;
					}
					else if (evnt.val == GAME_START)
					{
						vSerialPutString(NULL, "game\n", 5);
						score_start ();
						disp_mode = DISPLAY_GAME;
					}
					break;
				case EVENT_SCORE:
					if (disp_mode != DISPLAY_GAME)
					{
						continue;
					}
					break;
			}
		}

		if (disp_mode == DISPLAY_MENU)
		{
			display_func = &menu_display;
		}
		else if (disp_mode == DISPLAY_GAME)
		{
			display_func = &score_display;
			score_lock ();
		}

		u8g_FirstPage(&u8g);
		do
		{
			(*display_func) (&u8g);
		} while ( u8g_NextPage( &u8g ) );
		
		if (disp_mode == DISPLAY_GAME)
		{
			score_unlock ();
		}			
	}
}